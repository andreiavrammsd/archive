package cluster

type Client interface {
	Connect() error
	Close() error
	Status() (status Status, err error)
	Ping() error
	Sync(s Sync) error
	Work(task string, arg interface{}) (interface{}, error)
	WorkAsync(task string, arg interface{}) Wait
	Register(addr string) error
	Unregister(addr string) error
	AddNode(addr string) error
	Address() string
}

type Sync struct {
	TaskCount int
}

type Wait interface {
	Wait()
	Result() interface{}
	Error() error
}

type Status struct {
	Nodes []NodeInfo
}

type NodeInfo struct {
	Address   string
	TaskCount int
}

type ClientFactory func(addr string) Client
