package main

import (
	"testing"

	"github.com/andreiavrammsd/conlog/packages/netproc"
	"github.com/stretchr/testify/assert"
	"google.golang.org/grpc"
)

var GrpcDialFunctionCalls int

func TestNewCollector(t *testing.T) {
	ch := make(chan netproc.Processes)
	grpcDial = func(target string, opts ...grpc.DialOption) (*grpc.ClientConn, error) {
		GrpcDialFunctionCalls++
		return &grpc.ClientConn{}, nil
	}

	collect, err := NewCollector(ch, Client{})
	assert.IsType(t, &Collect{}, collect)
	assert.Nil(t, err)
	assert.Equal(t, 1, GrpcDialFunctionCalls)
}

func TestCollector_AddToQueueAndReadFromQueue(t *testing.T) {
	ch := make(chan netproc.Processes)
	collect, err := NewCollector(ch, Client{})

	assert.Nil(t, err)

	go func() {
		for {
			ps := collect.ReadFromQueue()
			assert.Len(t, ps, 1)
			assert.Equal(t, 1, ps["hash"].PID)
		}
	}()

	p := netproc.Process{}
	p.PID = 1
	collect.AddToQueue(netproc.Processes{"hash": p})
}
