package main

import (
	"context"
	"log"

	pb "github.com/andreiavrammsd/conlog/packages/collector"
	"github.com/andreiavrammsd/conlog/packages/config"
	"github.com/andreiavrammsd/conlog/packages/netproc"
	"github.com/golang/protobuf/ptypes"
	"google.golang.org/grpc"
)

type Collector interface {
	Close()
	AddToQueue(ps netproc.Processes)
	ReadFromQueue() netproc.Processes
	Send(ps netproc.Processes) error
}

type Collect struct {
	ch        chan netproc.Processes
	client    Client
	collector pb.CollectorClient
	conn      *grpc.ClientConn
}

func (c *Collect) Close() {
	c.conn.Close()
}

func (c *Collect) AddToQueue(ps netproc.Processes) {
	c.ch <- ps
}

func (c *Collect) ReadFromQueue() netproc.Processes {
	return <-c.ch
}

func (c *Collect) Send(ps netproc.Processes) error {
	collector := &pb.CollectorRequest{
		Client: &pb.CollectorRequest_Client{
			Id:       c.client.ID,
			Secret:   c.client.Secret,
			Hostname: c.client.Hostname,
			Uuid:     c.client.UUID,
			Uname:    c.client.Uname,
		},
		Processes: []*pb.CollectorRequest_Process{},
	}

	for _, p := range ps {
		time, _ := ptypes.TimestampProto(p.Time)
		coll := &pb.CollectorRequest_Process{
			Protocol:    p.Protocol,
			UserID:      p.UserID,
			User:        p.User,
			Name:        p.Name,
			PID:         int32(p.PID),
			Exe:         p.Exe,
			State:       p.State,
			IP:          p.IP,
			Port:        p.Port,
			ForeignIP:   p.ForeignIP,
			ForeignPort: p.ForeignPort,
			Inode:       int32(p.Inode),
			Time:        time,
		}
		collector.Processes = append(collector.Processes, coll)
	}

	resp, err := c.collector.CreateCollector(context.Background(), collector)
	if err != nil {
		return err
	}

	if resp.Success {
		log.Printf("Sent %d processes", len(collector.Processes))
	}

	return nil
}

var grpcDial = grpc.Dial

func NewCollector(ch chan netproc.Processes, client Client) (Collector, error) {
	// Set up a connection to the gRPC server
	conn, err := grpcDial(config.COLLECTOR_URL, grpc.WithInsecure())
	if err != nil {
		return &Collect{}, err
	}

	// Create a new CollectorClient
	collector := pb.NewCollectorClient(conn)

	return &Collect{
		ch,
		client,
		collector,
		conn,
	}, nil
}
