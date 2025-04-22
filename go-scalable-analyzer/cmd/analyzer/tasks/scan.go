package tasks

import (
	"math"
	"os"
	"runtime"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/storage"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/logger"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue"
)

type Analyzer interface {
	Analyze(analyzer.Task) error
}

type AnalyzeFile struct {
	Reader   queue.Reader
	Writer   queue.Writer
	Updater  storage.Writer
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
		s.Logger.Error("analyzer decoder error", err)
		return err
	}

	// generate unique id from task/issue - avoid duplicates
	// if analyze fails and restarts
	if err := s.Analyzer.Analyze(task); err != nil {
		s.Logger.Error("analyzer error", err)
		return err
	}

	obj := struct {
		Status string `bson:"status"`
	}{Status: "DONE"}
	if err := s.Updater.Update(task.AnalyzeID, obj); err != nil {
		return err
	}

	// TBD: what to do if cannot delete
	if err := os.Remove(task.LocalFile); err != nil {
		s.Logger.Errorf("could not delete local file: %s", err)
	}

	return nil
}
