package main

import (
	"fmt"
	"log"
	"net"

	"github.com/andreiavrammsd/conlog/packages/collector"
	"github.com/andreiavrammsd/conlog/packages/config"
	"github.com/andreiavrammsd/conlog/packages/e"
	"github.com/streadway/amqp"
	"google.golang.org/grpc"
)

func main() {
	// Set TCP connection for gRPC
	lis, err := net.Listen("tcp", config.COLLECTOR_SERVER_ADDRESS)
	e.LogFatalIfError(err)

	// Connect to rabbit
	url := fmt.Sprintf(
		"amqp://%s:%s@%s:%d/",
		config.QUEUER_USERNAME,
		config.QUEUER_PASSWORD,
		config.QUEUER_HOST,
		config.QUEUER_PORT,
	)
	conn, err := amqp.Dial(url)
	e.LogFatalIfError(err)
	defer conn.Close()

	// Channel for incoming processes
	channel := make(chan []byte)

	// Send processes to Queuer
	for i := 1; i <= config.COLLECTOR_QUEUE_PUBLISHER_COUNT; i++ {
		go publisher(conn, channel)
	}

	// Setup Collector RPC server to receive processes from Agent
	s := grpc.NewServer()
	rpc := &collector.CollectorRPC{Channel: channel}
	collector.RegisterCollectorServer(s, rpc)

	log.Printf(
		"Ready on %s with %d Queuer publisher(s)",
		config.COLLECTOR_SERVER_ADDRESS,
		config.COLLECTOR_QUEUE_PUBLISHER_COUNT,
	)
	log.Fatal(s.Serve(lis))
}
