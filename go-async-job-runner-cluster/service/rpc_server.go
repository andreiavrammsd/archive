package service

import (
	"net/http"
	"github.com/gorilla/mux"
	"github.com/gorilla/rpc"
	"github.com/gorilla/rpc/json"
	"fmt"
)

type RPCServer interface {
	ListenAndServe() error
}

type RPCServerConfig struct {
	Port     int
	Path     string
	Services []interface{}
}

type rpcServer struct {
	config RPCServerConfig
	codecs []string
}

func (s *rpcServer) ListenAndServe() error {
	server := rpc.NewServer()

	for _, c := range s.codecs {
		server.RegisterCodec(json.NewCodec(), c)
	}

	for _, service := range s.config.Services {
		server.RegisterService(service, "")
	}

	r := mux.NewRouter()
	r.Handle(s.config.Path, server)
	return http.ListenAndServe(fmt.Sprintf(":%d", s.config.Port), r)
}

func NewRPCServer(c RPCServerConfig) RPCServer {
	s := &rpcServer{}
	s.config = c
	s.codecs = []string{
		"application/json;charset=UTF-8",
		"application/json",
	}
	return s
}
