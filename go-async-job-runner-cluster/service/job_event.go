package service

import (
	"log"

	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type JobEvent interface {
	Start(job m.Job) error
	End(job m.Job) error
}

type jobEvent struct {
	WS      Worker
	client  RPCClient
	workers []m.Worker
}

type SyncJobArgs struct {
	m.Job
}

func (jer *jobEvent) Start(job m.Job) error {
	return jer.rpc("Sync.JobStart", job)
}

func (jer *jobEvent) End(job m.Job) error {
	return jer.rpc("Sync.JobEnd", job)
}

func (jer *jobEvent) rpc(method string, job m.Job) error {
	for _, w := range jer.workers {
		go func(workerID uint64) {
			worker, err := jer.WS.GetByID(workerID)
			if err != nil {
				log.Println(err)
			}

			request := ClientRequest{
				worker.GetURL(),
				method,
				SyncJobArgs{job},
			}
			err = jer.client.Call(request, nil)
			if err != nil {
				log.Println(err)
			}
		}(w.ID)
	}

	return nil
}

func NewJobEventRunner(ws Worker, client RPCClient, workers []m.Worker) JobEvent {
	return &jobEvent{ws, client, workers}
}
