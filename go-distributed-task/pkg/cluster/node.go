package cluster

import (
	"fmt"
)

// TaskFunc is the function signature for a task
type TaskFunc func(interface{}) (interface{}, error)

// Node represents an instance of a worker node
type Node struct {
	taskCount int
	client    Client
	tasks     map[string]TaskFunc
}

// Work executes the requested task
func (n *Node) Work(task string, arg interface{}) (interface{}, error) {
	f, ok := n.tasks[task] // can it race?
	if !ok {
		return nil, fmt.Errorf("unknown task %s", task)
	}
	return f(arg)
}

func (n *Node) Add(addr string) error {
	return nil
	//return n.client.Add(addr)
}

// Sync send current info of current node to all nodes
// TODO: exclude current node
func (n *Node) Sync(s Sync) error {
	return n.client.Sync(s)
}

// Address of node
func (n *Node) Address() string {
	return n.client.Address()
}

// TaskCount is the number of tasks currently being executed on this node
func (n *Node) TaskCount() int {
	return n.taskCount
}
