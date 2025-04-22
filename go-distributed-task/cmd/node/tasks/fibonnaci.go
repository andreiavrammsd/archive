package tasks

import (
	"errors"
	"time"
)

const FibonacciTask = "fibonacci"

func Fibonacci(arg interface{}) (interface{}, error) {
	n := arg.(int)
	return fibonacci(n)
}

func fibonacci(n int) (result []int, err error) {
	time.Sleep(time.Second * 10)
	if n == 0 {
		err = errors.New("invalid value")
		return
	}

	if n == 1 {
		return []int{1}, nil
	}

	if n == 2 {
		return []int{1, 1}, nil
	}

	result = []int{1, 1, 2}
	for i := 2; i < n-1; i++ {
		result = append(result, result[i-1]+result[i])
	}

	return
}
