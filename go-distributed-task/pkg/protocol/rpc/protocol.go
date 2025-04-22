package rpc

type StatusArgs struct {
}

type StatusReply struct {
	Nodes []NodeInfo
}

type SyncArgs struct {
	TaskCount int
}

type NodeInfo struct {
	Address   string
	TaskCount int
}

type RegisterArgs struct {
	Address string
}

type UnregisterArgs struct {
	Address string
}

type AddNodeArgs struct {
	Address string
}

type WorkArgs struct {
	Task string
	Arg  interface{}
}

type WorkReply struct {
	Task   string
	Arg    interface{}
	Result interface{}
}
