package main

import (
	"context"
	"log"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/andreiavrammsd/tasks-runner/cmd/server/tasks"
	"github.com/andreiavrammsd/tasks-runner/pkg/analyzer"
	"github.com/andreiavrammsd/tasks-runner/pkg/logger"
	"github.com/andreiavrammsd/tasks-runner/pkg/queue/rabbitmq"
	"github.com/andreiavrammsd/tasks-runner/pkg/storage/mongo"
	"github.com/andreiavrammsd/tasks-runner/pkg/taskrunner"
)

func main() {
	go func() {
		log.Fatal(http.ListenAndServe("localhost:8080", nil))
	}()

	// Config
	hostname, err := os.Hostname()
	if err != nil {
		log.Fatal(err)
	}

	config := Config{
		Mongo: Mongo{
			Host:           "localhost:27017",
			Username:       "root",
			Password:       "root",
			Database:       "tasks",
			Collection:     "issues",
			ConnectTimeout: time.Second * 10,
			QueryTimeout:   time.Second * 10,
		},
		Rabbit: rabbitmq.ClientConfig{
			Host:     "localhost:5672",
			Username: "root",
			Password: "root",
		},
		QueueDownload: "download",
		QueueAnalyze:  "analyze:" + hostname,
		QueueIssues:   "issues",
	}

	// Logger
	logging := logger.New(os.Stdout, os.Stderr)

	// Panic handler
	defer func() {
		if err := recover(); err != nil {
			logging.Errorf("panic: %v", err)
		}
	}()

	// Rabbit client
	rabbitClient, err := rabbitmq.NewClient(config.Rabbit)
	if err != nil {
		logging.Fatal(err)
	}

	// Mongo client
	ctx, cancel := context.WithTimeout(context.Background(), config.Mongo.ConnectTimeout)
	defer func() {
		cancel()
	}()
	mongoClient, err := mongo.NewClient(ctx, mongo.ClientConfig{
		Host:     config.Mongo.Host,
		Username: config.Mongo.Username,
		Password: config.Mongo.Password,
	})
	if err != nil {
		logging.Fatal(err)
	}

	// Health check
	health := tasks.Health{
		Period: time.Second * 5,
		Services: []tasks.HealthService{
			&tasks.MongoHealth{Client: mongoClient},
		},
	}
	if err := health.Ping(); err != nil {
		logging.Fatal(err)
	}

	// Incoming for file download
	incomingDownload, err := rabbitmq.NewReader(&rabbitmq.ReaderConfig{
		Connection: rabbitClient,
		Queue:      config.QueueDownload,
		Logger:     logging,
	})
	if err != nil {
		logging.Fatal(err)
	}

	// Analyzer
	issueAnalyzer := analyzer.New()

	// Outgoing for issue save to queue
	outgoingForIssue, err := rabbitmq.NewWriter(&rabbitmq.WriterConfig{
		Connection: rabbitClient,
		Queue:      config.QueueIssues,
	})
	if err != nil {
		logging.Fatal(err)
	}

	// Incoming for issue save to db
	incomingIssue, err := rabbitmq.NewReader(&rabbitmq.ReaderConfig{
		Connection: rabbitClient,
		Queue:      config.QueueIssues,
		Logger:     logging,
	})
	if err != nil {
		logging.Fatal(err)
	}

	analyzeIssueWriter := mongo.NewBatchWriter(&mongo.WriterConfig{
		Client:     mongoClient,
		Database:   config.Mongo.Database,
		Collection: config.Mongo.Collection,
	})

	// Tasks setup
	taskList := []taskrunner.Task{
		&tasks.DownloadAndAnalyze{
			Reader:   incomingDownload,
			Analyzer: issueAnalyzer,
			Writer:   outgoingForIssue,
		},

		&tasks.SaveIssueBatch{
			Reader:        incomingIssue,
			Writer:        analyzeIssueWriter,
			WriterTimeout: config.Mongo.QueryTimeout,
			BatchLimit:    10,
		},
		&health,
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
	logging.Info("Goodbye...")
}
