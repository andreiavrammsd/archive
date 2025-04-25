package tasks

import (
	"context"
	"math"
	"runtime"
	"time"

	"github.com/andreiavrammsd/tasks-runner/pkg/analyzer"

	"github.com/andreiavrammsd/tasks-runner/pkg/queue"
	"github.com/andreiavrammsd/tasks-runner/pkg/storage"
)

type SaveIssue struct {
	Reader        queue.Reader
	Writer        storage.Writer
	WriterTimeout time.Duration
}

func (s *SaveIssue) Concurrency() uint {
	return uint(math.Max(1, float64(runtime.NumCPU())-2))
}

func (s *SaveIssue) Run() error {
	return s.Reader.Read(s.save)
}

func (s *SaveIssue) save(data []byte) error {
	issue := analyzer.Issue{}
	if err := issue.Decode(data); err != nil {
		return err
	}

	ctx, cancel := context.WithTimeout(context.Background(), s.WriterTimeout)
	defer cancel()

	if err := s.Writer.Write(ctx, issue); err != nil {
		return err
	}

	return nil
}
