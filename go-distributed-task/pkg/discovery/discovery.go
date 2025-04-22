package discovery

import (
	"errors"
	"fmt"
	"log"

	"github.com/andreiavrammsd/dtask/pkg/cluster"
)

type Register struct {
	Start     int
	Count     int
	NewClient cluster.ClientFactory
	Cluster   *cluster.Cluster
}

func (r *Register) FindFreeAddress() (string, error) {
	for port := r.Start; port < r.Start+r.Count; port++ {
		nodeAddr := fmt.Sprintf(":%d", port)

		c := r.NewClient(nodeAddr)
		if err := c.Connect(); err != nil {
			return nodeAddr, nil
		}
	}

	return "", errors.New("could not find free address")
}

func (r *Register) Register(currentNodeAddr string) error {
	if currentNodeAddr == "" {
		return errors.New("current address missing")
	}

	// Scan for nodes
	for port := r.Start; port < r.Start+r.Count; port++ {
		addr := fmt.Sprintf(":%d", port)

		if addr == currentNodeAddr {
			continue
		}

		log.Println("trying to register to ", addr)
		newClient := r.NewClient(addr)
		if err := newClient.Connect(); err != nil {
			log.Println("could not register to", addr)
			continue
		}

		// Found another node, register to it
		if err := newClient.Register(currentNodeAddr); err != nil {
			log.Println("register to all register", err, currentNodeAddr)
			continue
		}
		log.Println("registered self to ", addr)

		if err := newClient.Close(); err != nil {
			log.Println("register to all close", err)
			continue
		}
		break
	}

	return nil
}
