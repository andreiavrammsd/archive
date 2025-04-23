package collector

import (
	"github.com/golang/protobuf/proto"
	"golang.org/x/net/context"
	"log"
)

// collectorRPC is used to implement Collector.CollectorServer
type CollectorRPC struct {
	Channel chan []byte
}

// CreateCollector creates a new Collector
func (s *CollectorRPC) CreateCollector(ctx context.Context, in *CollectorRequest) (*CollectorResponse, error) {
	//log.Printf("Received %d processes from host %s", len(in.Processes), in.Client.Hostname)

	out, err := proto.Marshal(in)
	if err != nil {
		log.Fatalln("Failed to encode data:", err)
	}

	s.Channel <- out

	return &CollectorResponse{Id: in.Client.Id, Success: true}, nil
}
