package cluster

import (
	"fmt"
	"log"
	"sort"
	"sync"
)

// Cluster manages the actions of all cluster nodes.
// The list of nodes is sorted by number of tasks
type Cluster struct {
	nodes     []*Node
	lock      sync.RWMutex
	Tasks     map[string]TaskFunc
	NewClient ClientFactory
	Address   string
}

func (c *Cluster) Start() error {
	return nil
}

// Register adds a new node client to current node.
func (c *Cluster) Register(addr string) error {

	cl := c.NewClient(addr)
	if err := cl.Connect(); err != nil {
		return err
	}

	c.Nodes(func(node *Node) bool {
		fmt.Println("add node", node.Address(), c.Address, addr)
		if node.Address() != c.Address {
			if err := node.client.AddNode(addr); err != nil {
				log.Fatal("register add node", err)
			}
		}
		return true
	})

	node := Node{
		client: cl,
		tasks:  c.Tasks,
	}

	c.lock.Lock()
	c.nodes = append(c.nodes, &node)
	c.lock.Unlock()

	return nil
}

func (c *Cluster) AddNode(addr string) error {
	cl := c.NewClient(addr)
	if err := cl.Connect(); err != nil {
		return err
	}

	node := Node{
		client: cl,
		tasks:  c.Tasks,
	}

	c.lock.Lock()
	c.nodes = append(c.nodes, &node)
	c.lock.Unlock()

	return nil
}

func (c *Cluster) RemoveNode(addr string) error {
	c.lock.Lock()
	defer c.lock.Unlock()

	for i := 0; i < len(c.nodes); i++ {
		if c.nodes[i].Address() == addr {
			c.nodes = append(c.nodes[:i-1], c.nodes[i:]...)
			return nil
		}
	}

	return nil
}

func (c *Cluster) Unregister(addr string) error {
	c.Nodes(func(node *Node) bool {
		if node.Address() != addr {
			if err := node.client.Unregister(addr); err != nil {
				log.Fatal(err)
			}
		}
		return true
	})
	return nil
}

// Sync sorts list of nodes from current node by number of tasks.
// TODO: also sync remote all nodes
func (c *Cluster) Sync() {
	sort.Slice(c.nodes, func(i, j int) bool {
		return c.nodes[i].taskCount < c.nodes[j].taskCount //race
	})
}

// syncAll sends current node information to all nodes.
func (c *Cluster) syncAll() {
	for i := 0; i < len(c.nodes); i++ {
		go func(node *Node) {
			data := Sync{TaskCount: node.taskCount} //raceX
			if err := node.Sync(data); err != nil {
				log.Println("sync err", node.Address())
			}
		}(c.nodes[i])
	}
}

// Work executes the task on selected node.
func (c *Cluster) Work(task string, arg interface{}) (result interface{}, err error) {
	c.lock.Lock()
	c.Sync()
	c.syncAll()
	node := c.nodes[0]
	node.taskCount++
	c.lock.Unlock()

	result, err = node.Work(task, arg)

	c.lock.Lock()
	c.Sync()
	c.syncAll()
	node.taskCount-- //raceX
	c.lock.Unlock()

	return
}

// Len returns the total number of nodes
func (c *Cluster) Len() int {
	c.lock.RLock()
	defer c.lock.RUnlock()
	return len(c.nodes)
}

// Nodes iterates all the nodes in cluster
func (c *Cluster) Nodes(f func(*Node) bool) {
	c.lock.Lock()
	for i := 0; i < len(c.nodes); i++ {
		if !f(c.nodes[i]) {
			break
		}
	}
	c.lock.Unlock()
}
