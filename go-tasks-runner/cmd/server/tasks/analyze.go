package tasks

import (
	"log"
	"math"
	"os"
	"runtime"

	"github.com/andreiavrammsd/tasks-runner/pkg/analyzer"
	"github.com/andreiavrammsd/tasks-runner/pkg/logger"
	"github.com/andreiavrammsd/tasks-runner/pkg/queue"
)

type Analyzer interface {
	Analyze(analyzer.Task, queue.Writer) error
}

type AnalyzeFile struct {
	Reader   queue.Reader
	Writer   queue.Writer
	Analyzer Analyzer
	Logger   logger.Logger
}

func (s *AnalyzeFile) Concurrency() uint {
	return uint(math.Max(1, float64(runtime.NumCPU())-2))
}

func (s *AnalyzeFile) Run() error {
	return s.Reader.Read(s.analyze)
}

func (s *AnalyzeFile) analyze(data []byte) error {
	task := analyzer.Task{}
	if err := task.Decode(data); err != nil {
		log.Println(err)
		return err
	}

	log.Printf("Analyzint file %s...\n", task.LocalFile)

	// generate unique id from task/issue - avoid duplicates
	// if analyze fails and restarts
	if err := s.Analyzer.Analyze(task, s.Writer); err != nil {
		//s.Logger.Error("analyzer error", err)
		return err
	}

	// TBD: what to do if cannot delete
	if err := os.Remove(task.LocalFile); err != nil {
		s.Logger.Errorf("could not delete local file: %s", err)
	}

	return nil
}
