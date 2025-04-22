package rpc

import (
	"fmt"
	"net"
	"net/http"
	"net/rpc"

	"github.com/andreiavrammsd/dtask/pkg/cluster"
)

type TaskServer struct {
	cluster *cluster.Cluster
}

func (s *TaskServer) SendToNodes(args *RegisterArgs, reply *bool) error {
	return nil
}

func (s *TaskServer) Register(args *RegisterArgs, reply *bool) error {
	if err := s.cluster.Register(args.Address); err != nil {
		return err
	}

	*reply = true
	return nil
}

func (s *TaskServer) Unregister(args *RegisterArgs, reply *bool) error {
	if err := s.cluster.RemoveNode(args.Address); err != nil {
		return err
	}

	*reply = true
	return nil
}

func (s *TaskServer) AddNode(args *AddNodeArgs, reply *bool) error {
	fmt.Println("add node server", args.Address)
	if err := s.cluster.AddNode(args.Address); err != nil {
		return err
	}

	*reply = true
	return nil
}

func (s *TaskServer) Status(args *StatusArgs, reply *StatusReply) error {
	r := StatusReply{
		Nodes: make([]NodeInfo, 0, s.cluster.Len()),
	}
	s.cluster.Nodes(func(node *cluster.Node) bool {
		r.Nodes = append(r.Nodes, NodeInfo{
			Address:   node.Address(),
			TaskCount: node.TaskCount(),
		})

		return true
	})
	*reply = r
	return nil
}

func (s *TaskServer) Ping(args *bool, reply *bool) error {
	return nil
}

func (s *TaskServer) Sync(args *SyncArgs, reply *bool) error {
	s.cluster.Sync()
	return nil
}

func (s *TaskServer) Work(args *WorkArgs, reply *WorkReply) (err error) {
	fmt.Println("work to do", args.Task)
	reply.Task = args.Task
	reply.Arg = args.Arg
	reply.Result, err = s.cluster.Work(args.Task, args.Arg)
	return
}

type Server struct {
	Addr    string
	Cluster *cluster.Cluster
}

func (s *Server) Serve() error {
	if err := rpc.Register(&TaskServer{
		cluster: s.Cluster,
	}); err != nil {
		return err
	}

	rpc.HandleHTTP()

	listener, err := net.Listen("tcp", s.Addr)
	if err != nil {
		return err
	}

	s.Cluster.Address = s.Addr

	return http.Serve(listener, nil)
}
