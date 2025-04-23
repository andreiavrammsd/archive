package main

import (
	"github.com/andreiavrammsd/conlog/packages/netproc"
)

type Pipeline struct {
	workers []Worker
}

func (pe *Pipeline) Run(ps netproc.Processes) error {
	var err error

	for _, w := range pe.workers {
		if len(ps) == 0 {
			break
		}
		ps, err = w.Work(ps)
	}

	return err
}

func NewPipeline(workers []Worker) Pipeline {
	return Pipeline{
		workers,
	}
}
