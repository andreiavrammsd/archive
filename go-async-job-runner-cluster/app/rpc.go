package app

import (
	e "github.com/andreiavrammsd/async-job-runner-cluster/endpoint"
)

type RPCServices []interface{}

func GetRPCServices(container ServiceContainer, config Config) RPCServices {
	services := make([]interface{}, 0)

	Cluster := new(e.Cluster)
	Cluster.WS = container.Worker
	Cluster.JS = container.Job
	services = append(services, Cluster)

	Worker := new(e.Worker)
	Worker.WS = container.Worker
	Worker.JS = container.Job
	services = append(services, Worker)

	Job := new(e.Job)
	Job.JS = container.Job
	Job.JV = container.JobValidator
	Job.JR = container.JobRunner
	Job.JRR = container.JobRemoteRunner
	Job.JWS = container.JobWorkerSelector
	services = append(services, Job)

	Sync := new(e.Sync)
	Sync.JS = container.Job
	services = append(services, Sync)

	return services
}
