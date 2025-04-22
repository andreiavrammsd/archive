package main

import (
	"log"
	"net/http"
	"os"
	"time"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/config"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/logger"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/taskrunner"
)

func main() {
	// Config
	conf, err := config.New()
	if err != nil {
		log.Fatal(err)
	}

	// API client setup
	api := APIClient{
		Addr:       conf.API.ClientAddress,
		HTTPClient: &http.Client{},
	}

	// Generate analyze request
	tasks := []taskrunner.Task{
		&GenerateClientAnalyzeRequest{
			MessageCount:         0,
			Pause:                time.Second * 1,
			PauseAfter:           100,
			ConcurrencyCount:     1,
			AnalyzeRequestSender: &api,
			Logger:               logger.New(os.Stdout, os.Stderr),
		},
	}

	// Start task runner
	runner := taskrunner.NewRunner(tasks, func(err error) {
		log.Println(err)
	})

	if err := runner.Run(); err != nil {
		log.Fatal(err)
	}

	select {}
}
