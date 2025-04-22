package service

import (
	"encoding/json"
	"log"
	"time"

	"github.com/andreiavrammsd/async-job-runner-cluster/jobs"
	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type JobRunner interface {
	Run(job m.Job) (m.Job, error)
}

type jobRunner struct {
	db  Database
	def jobs.Definition
	jer JobEvent
}

func (jr *jobRunner) Run(job m.Job) (m.Job, error) {
	// Get job definition
	definition := jr.def[job.Name]
	payload := definition.Payload
	json.Unmarshal(job.Payload, &payload)

	// Save job
	job.ID = uint64(time.Now().UnixNano())
	job.Time = time.Now().UTC()

	if err := jr.jer.Start(job); err != nil {
		return job, err
	}

	go func(job m.Job, definition jobs.DefinitionEntry, payload interface{}) {
		log.Printf("Starting job %d\n", job.ID)

		if err := definition.Job.Run(payload); err != nil {
			log.Println(err)
		}

		if err := jr.jer.End(job); err != nil {
			log.Println(err)
		}

		log.Printf("End of job %d\n", job.ID)
	}(job, definition, payload)

	return job, nil
}

func NewJobRunnerService(db Database, def jobs.Definition, jer JobEvent) JobRunner {
	jr := &jobRunner{}
	jr.db = db
	jr.def = def
	jr.jer = jer

	return jr
}
