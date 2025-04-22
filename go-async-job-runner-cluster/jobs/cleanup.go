package jobs

import (
	"time"
	"log"
)

type Cleanup struct {
	Path string
}

type CleanupPayload struct {
	Path string `json:"path"`
}

func (c *Cleanup) Run(payload interface{}) error {
	cleanup := payload.(*CleanupPayload)
	log.Println("Cleaning up... ", cleanup.Path)
	time.Sleep(time.Second * 2)
	return nil
}

