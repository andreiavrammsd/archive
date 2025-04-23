package main

import (
	"fmt"
	"log"
	"time"

	"github.com/andreiavrammsd/conlog/packages/e"
	"github.com/andreiavrammsd/conlog/packages/netproc"
	"gopkg.in/validator.v2"
)

const CONFIG_FILE = "conlog.yml"

func main() {
	config, err := NewConfig(CONFIG_FILE)
	e.LogFatalIfError(err)

	err = ValidateConfig(config)
	e.LogFatalIfError(err)

	client, err := NewClient(config)
	e.LogFatalIfError(err)

	// Init processes collector
	collect, err := NewCollector(make(chan netproc.Processes), client)
	defer collect.Close()
	e.LogFatalIfError(err)

	// Collector will wait for events and send them to remote
	go func(collect Collector) {
		for {
			ps := collect.ReadFromQueue()
			err := collect.Send(ps)
			if err != nil {
				log.Println(fmt.Sprintf("Could not send request: %v", err))
			}
		}
	}(collect)

	// Workers will analyze and transform network incoming processes
	workers := []Worker{
		&Syncer{
			make(map[string]bool),
		},
		&Computer{
			netproc.NewProcessIdentification(),
			netproc.NewProcessDirScanner(),
		},
		&Validator{validator.NewValidator()},
		&Emitter{
			collect,
		},
	}

	// Setup pipeline
	pipeline := NewPipeline(workers)

	// Channel to queue incoming processes on
	psch := make(chan netproc.Processes)

	// Wait for events and run workers on each processes list read from system
	go func(pipeline Pipeline) {
		for {
			ps := <-psch
			err := pipeline.Run(ps)
			if err != nil {
				log.Println(err)
			}
		}
	}(pipeline)

	// Continuously read network processes from system and send them to the pipeline
	pr := netproc.NewProcessReader()
	for {
		processes, err := pr.Read(config.Agent.Protocols)

		// Log errors
		if err != nil {
			log.Println(err)
		}

		// Send valid processes even if errors occurred
		if len(processes) > 0 {
			psch <- processes
		}

		time.Sleep(time.Millisecond * config.Agent.Delay)
	}
}
