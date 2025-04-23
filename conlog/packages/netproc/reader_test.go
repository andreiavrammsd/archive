package netproc

import (
	"errors"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewProcessReader(t *testing.T) {
	pr := NewProcessReader()
	assert.IsType(t, &processReader{}, pr)
}

type NetstatMock struct {
}

func (nsm NetstatMock) GetProcesses(proto string) (Processes, error) {
	processesByProtocols := map[string]Processes{
		"tcp": Processes{
			"bd3473415b8fb691fb05028de6a6c769": Process{},
			"5306a1ff063e9b3eec4d734391430947": Process{},
		},
		"tcp6": Processes{
			"dd3473415b8fb691fb05028de6a6c769": Process{},
		},
	}

	return processesByProtocols[proto], nil
}

func TestProcessReader_Read(t *testing.T) {
	ns := NetstatMock{}
	reader := processReader{ns}
	processes, err := reader.Read([]string{"tcp", "tcp6"})

	assert.Len(t, processes, 3)
	assert.Nil(t, err)
}

type NetstatMockWhenFails struct {
}

func (nsm NetstatMockWhenFails) GetProcesses(proto string) (Processes, error) {
	return Processes{}, errors.New("")
}

func TestProcessReader_ReadWhenNetstatFails(t *testing.T) {
	ns := NetstatMockWhenFails{}
	reader := processReader{ns}
	processes, err := reader.Read([]string{"tcp", "tcp6"})

	assert.Len(t, processes, 0)
	assert.NotNil(t, err)
}
