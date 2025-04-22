package service

import (
	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type Worker interface {
	GetAll() []m.Worker
	GetByID(id uint64) (m.Worker, error)
	CountAll() int
}

type worker struct {
	db Database
}

func (w *worker) GetAll() []m.Worker {
	workers := make([]m.Worker, 0)
	w.db.GetAll().Range(func(key, value interface{}) bool {
		w := value.(m.Worker)
		workers = append(workers, w)
		return true
	})
	return workers
}

func (w *worker) GetByID(id uint64) (m.Worker, error) {
	worker := m.Worker{}

	value, err := w.db.GetByKey(id)
	if err != nil {
		return worker, err
	}

	return value.(m.Worker), nil
}

func (w *worker) CountAll() int {
	var count int
	w.db.GetAll().Range(func(key, value interface{}) bool {
		count++
		return true
	})
	return count
}

func NewWorkerService(db Database) Worker {
	w := &worker{db}
	return w
}
