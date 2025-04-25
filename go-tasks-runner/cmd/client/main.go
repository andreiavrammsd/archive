package main

import (
	"log"
	"os"
	"time"

	"github.com/andreiavrammsd/tasks-runner/pkg/logger"
	"github.com/andreiavrammsd/tasks-runner/pkg/queue/rabbitmq"
	"github.com/andreiavrammsd/tasks-runner/pkg/taskrunner"
)

func main() {
	rabbitClientConfig := rabbitmq.ClientConfig{
		Host:     "localhost:5672",
		Username: "root",
		Password: "root",
	}
	rabbitClient, err := rabbitmq.NewClient(rabbitClientConfig)
	if err != nil {
		log.Fatal(err)
	}

	rabbitWriterConfig := rabbitmq.WriterConfig{
		Connection: rabbitClient,
		Queue:      "download",
	}
	rabbitWriter, err := rabbitmq.NewWriter(&rabbitWriterConfig)
	if err != nil {
		log.Fatal(err)
	}

	tasks := []taskrunner.Task{
		&GenerateClientMessages{
			MessageCount:     0,
			Pause:            time.Second * 3,
			PauseAfter:       3,
			ConcurrencyCount: 1,
			Writer:           rabbitWriter,
			Logger:           logger.New(os.Stdout, os.Stderr),
		},
	}

	runner := taskrunner.NewRunner(tasks, func(err error) {
		log.Println(err)
	})

	if err := runner.Run(); err != nil {
		log.Fatal(err)
	}

	select {}
}
