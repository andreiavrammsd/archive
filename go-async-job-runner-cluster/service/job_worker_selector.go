package service

import (
	"math"

	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type JobWorkerSelector interface {
	Select() m.Worker
}

type jobWorkerSelector struct {
	db Database
}

func (jws *jobWorkerSelector) Select() m.Worker {
	counter := math.MaxInt64
	worker := m.Worker{}

	jws.db.GetAll().Range(func(key, value interface{}) bool {
		w := value.(m.Worker)
		if w.JobsCount < counter {
			counter = w.JobsCount
			worker = w
		}
		return true
	})

	return worker
}

func NewJobWorkerSelectorService(db Database) JobWorkerSelector {
	return &jobWorkerSelector{db}
}
