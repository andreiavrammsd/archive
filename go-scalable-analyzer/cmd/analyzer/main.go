package main

import (
	"context"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/andreiavrammsd/scalableanalyzer/cmd/analyzer/tasks"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/config"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/health"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/logger"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue/rabbitmq"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/storage/mongo"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/taskrunner"
)

func main() {
	//go func() {
	//	log.Fatal(http.ListenAndServe("localhost:8080", nil))
	//}()

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

	// Rabbit client
	rabbitClient, err := rabbitmq.NewClient(rabbitmq.ClientConfig{
		Host:     conf.Rabbit.Host,
		Username: conf.Rabbit.Username,
		Password: conf.Rabbit.Password,
	})
	if err != nil {
		logging.Fatal(err)
	}

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

	// Health check
	healthCheck := health.Health{
		Period: time.Second * 5,
		Services: []health.Service{
			&health.Mongo{Client: mongoClient},
		},
	}
	if err = healthCheck.Ping(); err != nil {
		logging.Fatal(err)
	}

	// Incoming analyze request
	incomingForAnalyze := rabbitmq.NewReader(&rabbitmq.ReaderConfig{
		Connection: rabbitClient,
		Queue:      conf.QueueAnalyze,
		Logger:     logging,
	})

	// Outgoing analyze result save
	outgoingForIssue, err := rabbitmq.NewWriter(&rabbitmq.WriterConfig{
		Connection: rabbitClient,
		Queue:      conf.QueueIssues,
	})
	if err != nil {
		logging.Fatal(err)
	}

	// Analyzer
	issueAnalyzer := analyzer.New(outgoingForIssue)

	// Incoming analyze result for database save
	incomingIssue := rabbitmq.NewReader(&rabbitmq.ReaderConfig{
		Connection: rabbitClient,
		Queue:      conf.QueueIssues,
		Logger:     logging,
	})

	// Analyze done updater
	analyzeDoneUpdater := mongo.NewWriter(&mongo.WriterConfig{
		Client:     mongoClient,
		Database:   conf.DatabaseIssues,
		Collection: conf.CollectionAnalyzeRequests,
		Timeout:    conf.Mongo.QueryTimeout,
	})

	// Outgoing analyze result to database
	analyzeIssueWriter := mongo.NewWriter(&mongo.WriterConfig{
		Client:     mongoClient,
		Database:   conf.DatabaseIssues,
		Collection: conf.CollectionIssues,
		Timeout:    conf.Mongo.QueryTimeout,
	})

	//analyzeIssueWriter := mongo.NewBatchWriter(&mongo.WriterConfig{
	//	Client:     mongoClient,
	//	Database:   conf.DatabaseIssues,
	//	Collection: conf.CollectionIssues,
	//})

	// Tasks setup
	taskList := []taskrunner.Task{
		&tasks.AnalyzeFile{
			Reader:   incomingForAnalyze,
			Writer:   outgoingForIssue,
			Updater:  analyzeDoneUpdater,
			Analyzer: issueAnalyzer,
			Logger:   logging,
		},
		&tasks.SaveIssue{
			Reader: incomingIssue,
			Writer: analyzeIssueWriter,
		},
		//&tasks.SaveIssueBatch{
		//	Reader:        incomingIssue,
		//	Writer:        analyzeIssueWriter,
		//	WriterTimeout: conf.Mongo.QueryTimeout,
		//	BatchLimit:    10,
		//},
		&healthCheck,
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
