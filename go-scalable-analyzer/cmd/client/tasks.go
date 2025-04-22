package main

import (
	"time"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/logger"
)

type AnalyzeRequestSender interface {
	Send(r analyzer.Request) (err error)
}

type GenerateClientAnalyzeRequest struct {
	MessageCount         uint
	Pause                time.Duration
	PauseAfter           uint
	ConcurrencyCount     uint
	AnalyzeRequestSender AnalyzeRequestSender
	Logger               logger.Logger
}

func (c *GenerateClientAnalyzeRequest) Concurrency() uint {
	return c.ConcurrencyCount
}

func (c *GenerateClientAnalyzeRequest) Run() error {
	i := uint(0)
	for {
		message := analyzer.Request{
			Provider: "fs",
			Username: "",
			File:     "/tmp/file",
			Time:     time.Now().Unix(),
		}

		if err := c.AnalyzeRequestSender.Send(message); err != nil {
			c.Logger.Error(err)
			continue
		}

		c.Logger.Info("send", message)

		i++
		if c.PauseAfter > 0 && c.Pause > 0 {
			if i%c.PauseAfter == 0 {
				time.Sleep(c.Pause)
			}
		}

		if i == c.MessageCount {
			break
		}
	}

	return nil
}
