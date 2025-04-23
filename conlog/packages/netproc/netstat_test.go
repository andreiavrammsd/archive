package netproc

import (
	"errors"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
)

func TestNetstat_GetProcesses(t *testing.T) {
	nestatSample :=
		`  sl  local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt   uid  timeout inode ref pointer drops
0: 0100007F:44C3 00000000:0000 0A 00000000:00000000 00:00000000 00000000  1000        0 7019719 1 0000000000000000 100 0 0 10 0                   
1: 0100007F:F76E 00000000:0000 0A 00000000:00000000 00:00000000 00000000  1000        0 11995666 1 0000000000000000 100 0 0 10 0     
`
	readFile = func(filename string) ([]byte, error) {
		return []byte(nestatSample), nil
	}

	ns := netstat{}

	processes, err := ns.GetProcesses("tcp")
	assert.Len(t, processes, 2)
	assert.Nil(t, err)

	p0 := Process{}
	p0.Protocol = "tcp"
	p0.UserID = "1000"
	p0.User = ""
	p0.Name = ""
	p0.PID = -1
	p0.Exe = ""
	p0.State = "0A"
	p0.IP = "127.0.0.1"
	p0.Port = 17603
	p0.ForeignIP = "0.0.0.0"
	p0.ForeignPort = 0
	p0.Inode = 7019719

	p1 := Process{}
	p1.Protocol = "tcp"
	p1.UserID = "1000"
	p1.User = ""
	p1.Name = ""
	p1.PID = -1
	p1.Exe = ""
	p1.State = "0A"
	p1.IP = "127.0.0.1"
	p1.Port = 63342
	p1.ForeignIP = "0.0.0.0"
	p1.ForeignPort = 0
	p1.Inode = 11995666

	expected := Processes{
		"dfa0746dff7114afb0da8c644298ffef": p0,
		"380d32833543e79e9fe459c86af851fd": p1,
	}

	for hash, p := range processes {
		assert.Len(t, hash, 32)
		assert.IsType(t, Process{}, p)
		assert.IsType(t, time.Time{}, p.Time)

		e := expected[hash]
		e.Time = p.Time
		assert.Equal(t, e, p)
	}
}

func TestNetstat_GetProcessesWhenFileReadFails(t *testing.T) {
	readFile = func(filename string) ([]byte, error) {
		return []byte(""), errors.New("")
	}

	ns := netstat{}

	processes, err := ns.GetProcesses("tcp")
	assert.Len(t, processes, 0)
	assert.NotNil(t, err)
}

func TestNetstat_GetProcessesWhenGetInodeFails(t *testing.T) {
	nestatSample :=
		`  sl  local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt   uid  timeout inode ref pointer drops
0: 0100007F:44C3 00000000:0000 0A 00000000:00000000 00:00000000 00000000  1000        0 7019719 1 0000000000000000 100 0 0 10 0                   
`
	readFile = func(filename string) ([]byte, error) {
		return []byte(nestatSample), nil
	}

	strconvAtoi = func(s string) (int, error) {
		return 0, errors.New("")
	}

	ns := netstat{}

	processes, err := ns.GetProcesses("tcp")
	assert.Len(t, processes, 1)
	assert.Nil(t, err)

	p0 := Process{}
	p0.Protocol = "tcp"
	p0.UserID = "1000"
	p0.User = ""
	p0.Name = ""
	p0.PID = -1
	p0.Exe = ""
	p0.State = "0A"
	p0.IP = "127.0.0.1"
	p0.Port = 17603
	p0.ForeignIP = "0.0.0.0"
	p0.ForeignPort = 0
	p0.Inode = -1

	result := processes["9d54ad036b817824ef0504a176bbedcc"]
	p0.Time = result.Time
	assert.Equal(t, p0, result)
}
