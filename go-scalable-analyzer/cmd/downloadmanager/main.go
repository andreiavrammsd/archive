package main

import (
	// _ "net/http/pprof"
	"os"
	"os/signal"
	"syscall"

	"github.com/andreiavrammsd/scalableanalyzer/cmd/downloadmanager/tasks"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/config"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/logger"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue/rabbitmq"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/taskrunner"
)

func main() {
	// go func() {
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

	// Incoming file download request
	incomingDownload := rabbitmq.NewReader(&rabbitmq.ReaderConfig{
		Connection: rabbitClient,
		Queue:      conf.QueueDownload,
		Logger:     logging,
	})

	// Outgoing analyze request
	outgoingForAnalyze, err := rabbitmq.NewWriter(&rabbitmq.WriterConfig{
		Connection: rabbitClient,
		Queue:      conf.QueueAnalyze,
	})
	if err != nil {
		logging.Fatal(err)
	}

	// Run tasks
	taskList := []taskrunner.Task{
		&tasks.DownloadFile{
			Reader: incomingDownload,
			Writer: outgoingForAnalyze,
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
