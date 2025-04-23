package main

import (
	"errors"
	"strings"

	"github.com/andreiavrammsd/conlog/packages/netproc"
	"gopkg.in/validator.v2"
)

type Worker interface {
	Work(ps netproc.Processes) (netproc.Processes, error)
}

// Validator passes on only valid processes and logs invalid ones
type Validator struct {
	validator *validator.Validator
}

func (v *Validator) Work(ps netproc.Processes) (netproc.Processes, error) {
	valid := make(netproc.Processes)
	errorMessages := []string{}

	for hash, p := range ps {
		if err := v.validator.Validate(p); err != nil {
			errorMessages = append(errorMessages, err.Error())
			continue
		}

		valid[hash] = p
	}

	var err error
	if len(errorMessages) > 0 {
		err = errors.New(strings.Join(errorMessages, ", "))
	}

	return valid, err
}

// Syncer is a in memory database which filters already sent processes
type Syncer struct {
	memory map[string]bool
}

func (s *Syncer) Work(ps netproc.Processes) (netproc.Processes, error) {
	diff := make(netproc.Processes)
	for hash, p := range ps {
		if _, ok := s.memory[hash]; !ok {
			diff[hash] = p
			s.memory[hash] = true
		}
	}

	return diff, nil
}

// Computer adds extra info to processes (executables names, users names, pids)
type Computer struct {
	PI  netproc.ProcessIdentification
	PDS netproc.ProcessDirScanner
}

func (c *Computer) Work(ps netproc.Processes) (netproc.Processes, error) {
	dirs, err := c.PDS.Scan()
	if err != nil {
		return netproc.Processes{}, err
	}

	for hash, p := range ps {
		ps[hash] = c.PI.Identify(p, dirs)
	}

	return ps, nil
}

// Emitter sends processes to Collector channel
type Emitter struct {
	collector Collector
}

func (e *Emitter) Work(ps netproc.Processes) (netproc.Processes, error) {
	e.collector.AddToQueue(ps)
	return ps, nil
}
