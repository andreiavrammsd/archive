package app

import "github.com/andreiavrammsd/async-job-runner-cluster/service"

type App interface {
	Run() error
}

type app struct {
	server service.RPCServer
}

func (a *app) Run() error {
	return a.server.ListenAndServe()
}

func NewApp(server service.RPCServer) App {
	return &app{server}
}
