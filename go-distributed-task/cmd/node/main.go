package main

import (
	"flag"
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/andreiavrammsd/dtask/cmd/node/tasks"
	"github.com/andreiavrammsd/dtask/pkg/cluster"
	"github.com/andreiavrammsd/dtask/pkg/discovery"
	"github.com/andreiavrammsd/dtask/pkg/protocol/rpc"
)

func main() {
	addr := flag.String("addr", "", "")
	flag.Parse()
	nodeAddr := *addr

	// Client factory
	newClient := func(addr string) cluster.Client {
		return &rpc.Client{
			Addr: addr,
		}

		// include connect?
	}

	// Cluster
	dTaskCluster := &cluster.Cluster{
		Tasks:     tasks.List(),
		NewClient: newClient,
	}

	// Register node
	reg := &discovery.Register{
		Start:     8080,
		Count:     3,
		NewClient: newClient,
		Cluster:   dTaskCluster,
	}

	if nodeAddr == "" {
		address, err := reg.FindFreeAddress()
		if err != nil {
			log.Fatal(err)
		}

		nodeAddr = address
	}

	// Server
	server := rpc.Server{
		Addr:    nodeAddr,
		Cluster: dTaskCluster,
	}

	go func() {
		log.Fatal(server.Serve())
	}()

	log.Printf("Waiting for node %s to be up", nodeAddr)
	client := newClient(nodeAddr)
	for {
		if err := client.Connect(); err == nil {
			if err := client.Close(); err != nil {
				log.Println(err)
			}
			break
		}
	}

	log.Printf("Node %s is up", nodeAddr)

	// Register to self
	// use map for cluster node clients instead of slice?
	if err := dTaskCluster.Register(nodeAddr); err != nil {
		log.Fatalf("Cannot register node %s to self: %s", nodeAddr, err)
	}

	// Register current node to other nodes
	if err := reg.Register(nodeAddr); err != nil {
		log.Fatalf("Cannot register node %s to the others: %s", nodeAddr, err)
	}

	go func() {
		for {
			log.Printf("Number of nodes on %s: %d", nodeAddr, dTaskCluster.Len())
			time.Sleep(time.Second)
		}
	}()

	sig := make(chan os.Signal)
	signal.Notify(sig, os.Interrupt, syscall.SIGTERM)
	<-sig

	if err := dTaskCluster.Unregister(nodeAddr); err != nil {
		log.Println(err)
	}

	log.Println("Bye")
	// on exit, unregister self and close connections to others
}
