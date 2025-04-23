package main

import (
	"testing"

	"github.com/andreiavrammsd/conlog/packages/netproc"
	"github.com/stretchr/testify/assert"
)

var workerResult netproc.Processes

type TestWorker struct {
}

func (tw *TestWorker) Work(ps netproc.Processes) (netproc.Processes, error) {
	workerResult = ps
	return ps, nil
}

func TestNewPipeline(t *testing.T) {
	pipeline := NewPipeline([]Worker{&TestWorker{}})
	assert.IsType(t, Pipeline{}, pipeline)
	assert.Len(t, pipeline.workers, 1)
}

func TestPipeline_Run(t *testing.T) {
	pipeline := NewPipeline([]Worker{&TestWorker{}})
	workerResult = make(netproc.Processes)

	p1 := netproc.Process{}
	p1.PID = 1
	p2 := netproc.Process{}
	p1.PID = 2
	processes := netproc.Processes{
		"hash1": p1,
		"hash2": p2,
	}

	pipeline.Run(processes)

	assert.Len(t, workerResult, len(processes))
}

type TestWorkerBreak struct {
}

func (twb *TestWorkerBreak) Work(ps netproc.Processes) (netproc.Processes, error) {
	return netproc.Processes{}, nil
}

func TestPipeline_RunWhenWorkerReturnsNoProcesses(t *testing.T) {
	pipeline := NewPipeline([]Worker{&TestWorkerBreak{}, &TestWorker{}})
	workerResult = make(netproc.Processes)

	p1 := netproc.Process{}
	p1.PID = 1
	p2 := netproc.Process{}
	p1.PID = 2
	processes := netproc.Processes{
		"hash1": p1,
		"hash2": p2,
	}

	pipeline.Run(processes)

	assert.Len(t, workerResult, 0)
}
