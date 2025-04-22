package tasks

import (
	"math"
	"runtime"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/storage"
)

type SaveIssue struct {
	Reader queue.Reader
	Writer storage.Writer
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

	if _, err := s.Writer.Write(issue); err != nil {
		return err
	}

	return nil
}
