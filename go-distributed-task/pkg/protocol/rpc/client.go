package rpc

import (
	"fmt"
	"net/rpc"

	"github.com/andreiavrammsd/dtask/pkg/cluster"
)

// Client holds a connection to a node
type Client struct {
	Addr   string
	client *rpc.Client
}

func (c *Client) Connect() (err error) {
	c.client, err = rpc.DialHTTP("tcp", c.Addr)
	return
}

func (c *Client) Status() (status cluster.Status, err error) {
	args := StatusArgs{}
	reply := StatusReply{}
	if err = c.client.Call("TaskServer.Status", &args, &reply); err != nil {
		return
	}

	status = cluster.Status{Nodes: make([]cluster.NodeInfo, len(reply.Nodes))}
	for i := 0; i < len(reply.Nodes); i++ {
		status.Nodes[i] = cluster.NodeInfo{
			Address:   reply.Nodes[i].Address,
			TaskCount: reply.Nodes[i].TaskCount,
		}
	}

	return
}

func (c *Client) Ping() error {
	args := false
	reply := false
	return c.client.Call("TaskServer.Ping", &args, &reply)
}

func (c *Client) Sync(s cluster.Sync) error {
	args := SyncArgs{TaskCount: s.TaskCount}
	reply := false
	return c.client.Call("TaskServer.Sync", &args, &reply)
}

func (c *Client) Work(task string, arg interface{}) (interface{}, error) { // wait for work: sync/async work + payload
	args := WorkArgs{
		Task: task,
		Arg:  arg,
	}
	reply := WorkReply{
		Task:   task,
		Arg:    arg,
		Result: nil,
	}
	err := c.client.Call("TaskServer.Work", &args, &reply)
	return reply.Result, err
}

type wait struct {
	wait *rpc.Call
}

func (w *wait) Wait() {
	<-w.wait.Done
}

func (w *wait) Result() interface{} {
	return w.wait.Reply.(*WorkReply).Result
}

func (w *wait) Error() error {
	return w.wait.Error
}

func (c *Client) WorkAsync(task string, arg interface{}) cluster.Wait {
	args := WorkArgs{
		Task: task,
		Arg:  arg,
	}
	reply := WorkReply{
		Task:   task,
		Arg:    arg,
		Result: nil,
	}

	w := make(chan *rpc.Call, 10)
	call := c.client.Go("TaskServer.Work", &args, &reply, w)
	return &wait{call}
}

func (c *Client) Register(addr string) error {
	args := RegisterArgs{
		Address: addr,
	}
	reply := false
	return c.client.Call("TaskServer.Register", &args, &reply)
}

func (c *Client) Unregister(addr string) error {
	args := UnregisterArgs{
		Address: addr,
	}
	reply := false
	return c.client.Call("TaskServer.Unregister", &args, &reply)
}

func (c *Client) AddNode(addr string) error {
	args := AddNodeArgs{
		Address: addr,
	}
	reply := false
	fmt.Println("add node client", addr)
	return c.client.Call("TaskServer.AddNode", &args, &reply)
}

func (c *Client) Close() error {
	return c.client.Close()
}

func (c *Client) Address() string {
	return c.Addr
}
