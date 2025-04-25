package main

import (
	"encoding/json"
	"time"

	"github.com/andreiavrammsd/tasks-runner/pkg/downloadmanager"
	"github.com/andreiavrammsd/tasks-runner/pkg/logger"
	"github.com/andreiavrammsd/tasks-runner/pkg/queue"
)

type ClientMessage struct {
	Provider downloadmanager.Provider
	Username string
	File     string
	Time     int64
}

func (c ClientMessage) Encode() ([]byte, error) {
	return json.Marshal(c)
}

func (c ClientMessage) Decode(data []byte) error {
	return json.Unmarshal(data, &c)
}

type GenerateClientMessages struct {
	MessageCount     uint
	Pause            time.Duration
	PauseAfter       uint
	ConcurrencyCount uint
	Writer           queue.Writer
	Logger           logger.Logger
}

func (c *GenerateClientMessages) Concurrency() uint {
	return c.ConcurrencyCount
}

func (c *GenerateClientMessages) Run() error {
	i := uint(0)
	for {

		message := ClientMessage{
			Provider: "fs",
			Username: "",
			File:     "./bin/client",
			Time:     time.Now().Unix(),
		}

		if err := c.Writer.Write(message); err != nil {
			c.Logger.Error(err)
			continue
		}

		c.Logger.Info("send", message)

		if c.PauseAfter > 0 && c.Pause > 0 {
			if i%c.PauseAfter == 0 {
				time.Sleep(c.Pause)
			}
		}

		i++
		if i == c.MessageCount {
			break
		}
	}

	return nil
}
