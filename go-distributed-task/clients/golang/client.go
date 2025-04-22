package golang

import (
	"github.com/andreiavrammsd/dtask/cmd/node/tasks"
	"github.com/andreiavrammsd/dtask/pkg/cluster"
)

type Tasks struct {
	client cluster.Client
}

func (t *Tasks) Fibonacci(n int) ([]int, error) {
	result, err := t.client.Work(tasks.FibonacciTask, n)
	if err != nil {
		return nil, err
	}
	return result.([]int), nil
}

func New(client cluster.Client) *Tasks {
	return &Tasks{client: client}
}
