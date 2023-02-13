package main

import (
	"github.com/andreiavrammsd/go-live-reload/packages/chronos"
	"github.com/andreiavrammsd/go-live-reload/packages/random"
	"log"
	"time"
)

func main() {
	for {
		log.Println(random.GetRandomInt()*3, chronos.GetSeconds())
		time.Sleep(time.Second)
	}
}
