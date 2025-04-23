package netproc

import (
	"errors"
	"strings"
)

type processReader struct {
	ns Netstat
}

// Read retrieves network processes for requested protocols
func (pr *processReader) Read(protocols []string) (Processes, error) {
	processes := make(Processes)
	errorMessages := make([]string, 0)

	for _, proto := range protocols {
		ps, err := pr.ns.GetProcesses(proto)
		if err != nil {
			errorMessages = append(errorMessages, err.Error())
			continue
		}

		for hash, p := range ps {
			processes[hash] = p
		}
	}

	if len(errorMessages) > 0 {
		return processes, errors.New(strings.Join(errorMessages, "\n"))
	}

	return processes, nil
}

func NewProcessReader() ProcessReader {
	return &processReader{
		&netstat{},
	}
}
