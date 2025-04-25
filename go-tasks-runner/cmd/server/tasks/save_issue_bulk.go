package tasks

import (
	"context"
	"math"
	"runtime"
	"sync"
	"time"

	"github.com/andreiavrammsd/tasks-runner/pkg/analyzer"

	"github.com/andreiavrammsd/tasks-runner/pkg/queue"
	"github.com/andreiavrammsd/tasks-runner/pkg/storage"
)

type SaveIssueBatch struct {
	Reader        queue.Reader
	Writer        storage.BatchWriter
	WriterTimeout time.Duration
	BatchLimit    int
	batch         []interface{}
	lock          sync.Mutex
}

func (s *SaveIssueBatch) Concurrency() uint {
	return uint(math.Max(1, float64(runtime.NumCPU())-2))
}

func (s *SaveIssueBatch) Run() error {
	return s.Reader.Read(s.save)
}

func (s *SaveIssueBatch) save(data []byte) error {
	issue := analyzer.Issue{}
	if err := issue.Decode(data); err != nil {
		return err
	}

	s.lock.Lock()
	defer s.lock.Unlock()

	s.batch = append(s.batch, issue)

	if len(s.batch) == s.BatchLimit {
		ctx, cancel := context.WithTimeout(context.Background(), s.WriterTimeout)
		defer cancel()
		if err := s.Writer.Write(ctx, s.batch); err != nil {
			return err
		}
		s.batch = nil
	}

	return nil
}
