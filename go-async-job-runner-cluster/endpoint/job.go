package endpoint

import (
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"net/http"
	"time"

	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
	s "github.com/andreiavrammsd/async-job-runner-cluster/service"
)

type Job struct {
	JS  s.Job
	JV  s.Validator
	JR  s.JobRunner
	JRR s.JobRunner
	JWS s.JobWorkerSelector
}

type JobListArgs struct {
}

type JobListResult []m.Job

func (j *Job) List(r *http.Request, args *JobListArgs, result *JobListResult) error {
	jobs := j.JS.GetAll()
	*result = JobListResult(jobs)
	return nil
}

type JobRunArgs struct {
	Name    string          `json:"name"`
	Payload json.RawMessage `json:"payload"`
}

type JobRunResult struct {
	ID     uint64    `json:"id"`
	Worker uint64    `json:"worker"`
	Time   time.Time `json:"time"`
}

func (j *Job) Run(r *http.Request, args *JobRunArgs, result *JobRunResult) error {
	job := m.Job{}
	job.Name = args.Name
	job.Payload = args.Payload

	if err := j.JV.Validate(job); err != nil {
		log.Println(err, job)
		return errors.New(fmt.Sprintf("Job %s not found or invalid payload", job.Name))
	}

	worker := j.JWS.Select()
	job.WorkerID = worker.ID

	job, err := j.JRR.Run(job)

	if err != nil {
		log.Println(err)
		return errors.New(fmt.Sprintf("Cannot not run job %s", job.Name))
	}

	jrr := JobRunResult{
		job.ID,
		job.WorkerID,
		job.Time,
	}

	*result = JobRunResult(jrr)
	return nil
}

type JobRunOnWorkerArgs struct {
	*JobRunArgs
	Worker uint64
}
type JobRunOnWorkerResult JobRunResult

func (j *Job) RunOnWorker(r *http.Request, args *JobRunOnWorkerArgs, result *JobRunOnWorkerResult) error {
	job := m.Job{}
	job.Name = args.Name
	job.Payload = args.Payload
	job.WorkerID = args.Worker

	if err := j.JV.Validate(job); err != nil {
		log.Println(err, job)
		return errors.New(fmt.Sprintf("Job %s not found or invalid payload", job.Name))
	}

	job, _ = j.JR.Run(job)

	jrr := JobRunResult{
		job.ID,
		job.WorkerID,
		job.Time,
	}

	*result = JobRunOnWorkerResult(jrr)
	return nil
}
