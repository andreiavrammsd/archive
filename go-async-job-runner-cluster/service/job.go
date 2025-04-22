package service

import (
	"github.com/andreiavrammsd/async-job-runner-cluster/jobs"
	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type Job interface {
	GetAll() []m.Job
	CountAll() int
	Store(job m.Job)
	Delete(job m.Job)
}

type job struct {
	db  Database
	def jobs.Definition
}

func (j *job) GetAll() []m.Job {
	jobs := make([]m.Job, 0)

	j.db.GetAll().Range(func(key interface{}, value interface{}) bool {
		jobs = append(jobs, value.(m.Job))
		return true
	})
	return jobs
}

func (j *job) CountAll() int {
	return len(j.GetAll())
}

func (j *job) Store(job m.Job) {
	j.db.Store(job.ID, job)
}

func (j *job) Delete(job m.Job) {
	j.db.Delete(job.ID)
}

func NewJobService(db Database, def jobs.Definition) Job {
	j := &job{}
	j.db = db
	j.def = def
	return j
}
