package endpoint

import (
	"errors"
	"fmt"
	"log"
	"net/http"

	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
	s "github.com/andreiavrammsd/async-job-runner-cluster/service"
)

type Worker struct {
	WS s.Worker
	JS s.Job
}

type WorkerListArgs struct{}
type WorkerListResult []m.Worker

func (w *Worker) List(r *http.Request, args *WorkerListArgs, result *WorkerListResult) error {
	workers := w.WS.GetAll()
	jobs := w.JS.GetAll()

	for key, worker := range workers {
		for _, job := range jobs {
			if worker.ID == job.WorkerID {
				workers[key].JobsCount++
			}
		}
	}

	*result = WorkerListResult(workers)
	return nil
}

type WorkerGetArgs struct {
	ID uint64
}
type WorkerGetResult m.Worker

func (w *Worker) Get(r *http.Request, args *WorkerGetArgs, result *WorkerGetResult) error {
	worker, err := w.WS.GetByID(args.ID)

	if err != nil {
		message := fmt.Sprintf("Worker with id %d not found", args.ID)
		log.Println(message)
		return errors.New(message)
	}

	*result = WorkerGetResult(worker)
	return nil
}

type WorkerStatusArgs struct{}
type WorkerStatusResult bool

func (w *Worker) Status(r *http.Request, args *WorkerStatusArgs, result *WorkerStatusResult) error {
	*result = true
	return nil
}
