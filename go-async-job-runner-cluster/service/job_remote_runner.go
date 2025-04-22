package service

import (
	"encoding/json"

	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type jobRemoteRunner struct {
	client RPCClient
	WS     Worker
}

type req struct {
	Name     interface{}
	Payload  interface{}
	WorkerID interface{}
}

type JobRunOnWorkerArgs struct {
	Name    string          `json:"name"`
	Payload json.RawMessage `json:"payload"`
	Worker  uint64
}

func (jrr *jobRemoteRunner) Run(job m.Job) (m.Job, error) {
	worker, err := jrr.WS.GetByID(job.WorkerID)
	if err != nil {
		return job, err
	}

	request := ClientRequest{
		worker.GetURL(),
		"Job.RunOnWorker",
		JobRunOnWorkerArgs{
			job.Name,
			job.Payload,
			worker.ID,
		},
	}

	err = jrr.client.Call(request, &job)
	if err != nil {
		return job, err
	}

	return job, nil
}

func NewJobRemoteRunnerService(client RPCClient, ws Worker) JobRunner {
	return &jobRemoteRunner{client, ws}
}
