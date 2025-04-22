package tasks

import (
	"github.com/andreiavrammsd/dtask/pkg/cluster"
)

func List() map[string]cluster.TaskFunc {
	return map[string]cluster.TaskFunc{
		FibonacciTask: Fibonacci,
	}
}
