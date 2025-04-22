package service

import (
	"net"
	"log"
)

type Network interface {
	GetLocalAddress() string
}

type network struct {
}

func (n *network) GetLocalAddress() string {
	conn, err := net.Dial("udp", "8.8.8.8:80")
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()

	localAddr := conn.LocalAddr().(*net.UDPAddr)
	
	return localAddr.IP.String()
}

func NewNetworkService() Network {
	return &network{}
}
