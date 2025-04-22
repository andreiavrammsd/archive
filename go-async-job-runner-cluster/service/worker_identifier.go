package service

import (
	"errors"
	"fmt"
	"log"

	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type WorkerIdentifier interface {
	GetCurrent() m.Worker
}

type workerIdentifier struct {
	network       Network
	workers       []m.Worker
	currentWorker m.Worker
}

func (wi *workerIdentifier) GetCurrent() m.Worker {
	return wi.currentWorker
}

func (wi *workerIdentifier) findCurrentWorker() (m.Worker, error) {
	localAddress := wi.network.GetLocalAddress()

	currentWorker := m.Worker{}
	found := false
	for _, w := range wi.workers {
		if w.Address == localAddress {
			found = true
			currentWorker = w
		}
	}

	if !found {
		return currentWorker, errors.New(
			fmt.Sprintf(
				"Could not find worker with address %s.",
				localAddress,
			),
		)
	}

	return currentWorker, nil
}

func NewWorkerIdentifierService(network Network, workers []m.Worker) WorkerIdentifier {
	wi := &workerIdentifier{}
	wi.network = network
	wi.workers = workers

	currentWorker, err := wi.findCurrentWorker()
	if err != nil {
		log.Fatal(err)
	}

	wi.currentWorker = currentWorker

	return wi
}
