package tasks

import (
	"math"
	"runtime"
	"sync"
	"time"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/storage"
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
		if err := s.Writer.Write(s.batch); err != nil {
			return err
		}
		s.batch = nil
	}

	return nil
}
