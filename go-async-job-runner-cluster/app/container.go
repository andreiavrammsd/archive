package app

import (
	s "github.com/andreiavrammsd/async-job-runner-cluster/service"
)

type ServiceContainer struct {
	Job               s.Job
	JobValidator      s.Validator
	JobWorkerSelector s.JobWorkerSelector
	JobRunner         s.JobRunner
	JobRemoteRunner   s.JobRunner
	Worker            s.Worker
	Network           s.Network
	Syncer            s.Syncer
	WorkerIdentifier  s.WorkerIdentifier
	RPCServer         s.RPCServer
}

func NewServiceContainer(config Config) ServiceContainer {
	c := ServiceContainer{}

	// Database
	JobDB := s.NewDatabase()
	WorkerDB := s.NewDatabase()

	// Network
	c.Network = s.NewNetworkService()

	// Syncer
	sc := s.SyncerConfig{Port: config.Server.Port, Path: config.Server.Path, Workers: config.Workers}
	c.Syncer = s.NewSyncService(WorkerDB, sc)

	// Worker
	c.Worker = s.NewWorkerService(WorkerDB)
	c.WorkerIdentifier = s.NewWorkerIdentifierService(c.Network, config.Workers)

	// Job
	c.Job = s.NewJobService(JobDB, config.JobsDefinition)
	c.JobValidator = s.NewJobValidatorService(config.JobsDefinition)
	c.JobWorkerSelector = s.NewJobWorkerSelectorService(WorkerDB)
	rpcClient := s.NewRPCClient()
	c.JobRunner = s.NewJobRunnerService(
		JobDB,
		config.JobsDefinition,
		s.NewJobEventRunner(c.Worker, rpcClient, config.Workers),
	)
	c.JobRemoteRunner = s.NewJobRemoteRunnerService(rpcClient, c.Worker)

	// RCP Server
	serverConfig := s.RPCServerConfig{}
	serverConfig.Port = config.Server.Port
	serverConfig.Path = config.Server.Path
	serverConfig.Services = GetRPCServices(c, config)
	c.RPCServer = s.NewRPCServer(serverConfig)

	return c
}
