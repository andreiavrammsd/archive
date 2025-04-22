package main

import (
	"context"
	"encoding/json"
	"net/http"
	"os"
	"os/signal"
	"syscall"

	"github.com/andreiavrammsd/scalableanalyzer/cmd/api/tasks"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/config"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/logger"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue/rabbitmq"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/storage/mongo"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/taskrunner"
)

func main() {
	// Logger
	logging := logger.New(os.Stdout, os.Stderr)

	// Panic handler
	defer func() {
		if err := recover(); err != nil {
			logging.Errorf("panic: %v", err)
		}
	}()

	// Config
	conf, err := config.New()
	if err != nil {
		logging.Fatal(err)
	}

	// Input
	concurrency := uint(4)
	input := make(chan analyzer.Request, concurrency)

	// Mongo client
	ctx, cancel := context.WithTimeout(context.Background(), conf.Mongo.ConnectTimeout)
	defer func() {
		cancel()
	}()
	mongoClient, err := mongo.NewClient(ctx, mongo.ClientConfig{
		Host:     conf.Mongo.Host,
		Username: conf.Mongo.Username,
		Password: conf.Mongo.Password,
	})
	if err != nil {
		logging.Fatal(err)
	}

	analyzeRequestWriter := mongo.NewWriter(&mongo.WriterConfig{
		Client:     mongoClient,
		Database:   conf.DatabaseIssues,
		Collection: conf.CollectionAnalyzeRequests,
		Timeout:    conf.Mongo.QueryTimeout,
	})

	// RabbitMQ setup
	rabbitClientConfig := rabbitmq.ClientConfig{
		Host:     conf.Rabbit.Host,
		Username: conf.Rabbit.Username,
		Password: conf.Rabbit.Password,
	}
	rabbitClient, err := rabbitmq.NewClient(rabbitClientConfig)
	if err != nil {
		logging.Fatal(err)
	}

	rabbitWriterConfig := rabbitmq.WriterConfig{
		Connection: rabbitClient,
		Queue:      conf.QueueDownload,
	}
	rabbitWriter, err := rabbitmq.NewWriter(&rabbitWriterConfig)
	if err != nil {
		logging.Fatal(err)
	}

	http.HandleFunc("/analyze", func(w http.ResponseWriter, r *http.Request) {
		if r.Method != http.MethodPost {
			w.WriteHeader(http.StatusBadRequest)
			logging.Errorf("bad method at /analyze: %s", r.Method)
			return
		}

		defer func() {
			if err := r.Body.Close(); err != nil {
				logging.Error(err)
			}
		}()

		analyzeRequest := analyzer.Request{}
		dec := json.NewDecoder(r.Body)
		if err := dec.Decode(&analyzeRequest); err != nil {
			w.WriteHeader(http.StatusBadRequest)
			logging.Errorf("cannot decode analyze request: %s", err)
			return
		}

		input <- analyzeRequest
	})

	go func() {
		logging.Infof("Listening on: %s", conf.API.ServerAddress)
		logging.Error(http.ListenAndServe(conf.API.ServerAddress, nil))
	}()

	// Run tasks
	taskList := []taskrunner.Task{
		&tasks.SendDownloadRequest{
			ConcurrencyCount: concurrency,
			Input:            input,
			DBWriter:         analyzeRequestWriter,
			QueueWriter:      rabbitWriter,
			Logger:           logging,
		},
	}

	tasksRunner := taskrunner.NewRunner(taskList, func(err error) {
		logging.Error(err)
	})
	if err := tasksRunner.Run(); err != nil {
		logging.Fatal(err)
	}

	sig := make(chan os.Signal, 1)
	signal.Notify(sig, os.Interrupt, syscall.SIGTERM)
	<-sig

	logging.Info("Stopping....")
	tasksRunner.Stop()
	tasksRunner.Wait()
}
