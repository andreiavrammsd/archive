package main

import (
	"fmt"
	"log"

	"github.com/andreiavrammsd/async-job-runner-cluster/app"
)

const CONFIG_FILE = "config.yml"

func main() {
	// Create app
	config, err := app.NewConfig(CONFIG_FILE)
	ExitOnError(err)
	container := app.NewServiceContainer(config)

	// Sync worker databases
	log.Println("Syncing worker with cluster...")
	syncer := container.Syncer
	syncer.SyncWorker()

	// Setup RPC server
	log.Println(fmt.Sprintf("Listening on port %d at path %s", config.Server.Port, config.Server.Path))
	AsyncJobRunnerCluster := app.NewApp(container.RPCServer)
	ExitOnError(AsyncJobRunnerCluster.Run())
}

func ExitOnError(err error) {
	if err != nil {
		log.Fatal(err)
	}
}
