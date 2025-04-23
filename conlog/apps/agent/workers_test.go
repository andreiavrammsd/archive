package main

import (
	"errors"
	"testing"

	"github.com/andreiavrammsd/conlog/packages/netproc"
	"github.com/stretchr/testify/assert"
)

func TestSyncer_Work(t *testing.T) {
	syncer := Syncer{make(map[string]bool)}

	p1 := netproc.Process{}
	p1.PID = 1

	p2 := netproc.Process{}
	p2.PID = 2

	processes := netproc.Processes{"hash1": p1, "hash2": p2}

	result, err := syncer.Work(processes)
	assert.Len(t, syncer.memory, len(processes))
	assert.Equal(t, p1, result["hash1"])
	assert.Equal(t, p2, result["hash2"])
	assert.Nil(t, err)

	result, err = syncer.Work(processes)
	assert.Len(t, syncer.memory, len(processes))
	assert.Len(t, result, 0)
	assert.Nil(t, err)

	p3 := netproc.Process{}
	p3.PID = 3
	processes["hash3"] = p3

	result, err = syncer.Work(processes)
	assert.Len(t, syncer.memory, len(processes))
	assert.Len(t, result, 1)
	assert.Equal(t, p3, result["hash3"])
	assert.Nil(t, err)
}

var IdentifyMethodCalls int

type ProcessIdentificationMock struct {
}

func (pim ProcessIdentificationMock) Identify(p netproc.Process, dirs map[string]string) netproc.Process {
	IdentifyMethodCalls++
	p.Exe = "exe name"
	return p
}

var ScanMethodCalls int

type ProcessDirScannerMock struct {
}

func (pdsm ProcessDirScannerMock) Scan() (map[string]string, error) {
	ScanMethodCalls++
	return make(map[string]string), nil
}

func TestComputer_Work(t *testing.T) {
	computer := Computer{ProcessIdentificationMock{}, ProcessDirScannerMock{}}
	IdentifyMethodCalls = 0
	ScanMethodCalls = 0

	p1 := netproc.Process{}
	p1.PID = 1

	processes := netproc.Processes{"hash1": p1}
	result, err := computer.Work(processes)
	p1.Exe = "exe name"

	assert.Len(t, result, len(processes))
	assert.Equal(t, p1, result["hash1"])
	assert.Equal(t, 1, IdentifyMethodCalls)
	assert.Equal(t, 1, ScanMethodCalls)
	assert.Nil(t, err)
}

type ProcessDirScannerFailMock struct {
}

func (pdsfm ProcessDirScannerFailMock) Scan() (map[string]string, error) {
	ScanMethodCalls++
	return make(map[string]string), errors.New("")
}

func TestComputer_WorkWhenProcessDirScannerFails(t *testing.T) {
	computer := Computer{ProcessIdentificationMock{}, ProcessDirScannerFailMock{}}
	ScanMethodCalls = 0

	p1 := netproc.Process{}
	p1.PID = 1

	processes := netproc.Processes{"hash1": p1}
	result, err := computer.Work(processes)
	p1.Exe = "exe name"

	assert.Len(t, result, 0)
	assert.Equal(t, 1, IdentifyMethodCalls)
	assert.Equal(t, 1, ScanMethodCalls)
	assert.NotNil(t, err)
}

var AddToQueueMethodCalls int

type CollecterMock struct {
}

func (cm *CollecterMock) Close() {
}
func (cm *CollecterMock) AddToQueue(ps netproc.Processes) {
	AddToQueueMethodCalls++
}
func (cm *CollecterMock) ReadFromQueue() netproc.Processes {
	return netproc.Processes{}
}
func (cm *CollecterMock) Send(ps netproc.Processes) error {
	return nil
}

func TestEmitter_Work(t *testing.T) {
	processes := netproc.Processes{}
	emitter := Emitter{&CollecterMock{}}
	result, err := emitter.Work(processes)
	assert.Equal(t, processes, result)
	assert.Equal(t, 1, AddToQueueMethodCalls)
	assert.Nil(t, err)
}
