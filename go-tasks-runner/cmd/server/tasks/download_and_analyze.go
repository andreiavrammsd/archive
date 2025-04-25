package tasks

import (
	"math"
	"os"
	"runtime"

	"github.com/andreiavrammsd/tasks-runner/pkg/analyzer"
	"github.com/andreiavrammsd/tasks-runner/pkg/queue"
)

type DownloadAndAnalyze struct {
	Reader   queue.Reader
	Analyzer Analyzer
	Writer   queue.Writer
}

func (d *DownloadAndAnalyze) Concurrency() uint {
	return uint(math.Max(1, float64(runtime.NumCPU())-2))
}

func (d *DownloadAndAnalyze) Run() error {
	return d.Reader.Read(d.run)
}

func (d *DownloadAndAnalyze) run(data []byte) error {
	message := ClientMessage{}
	if err := message.Decode(data); err != nil {
		return err
	}

	dm, err := getDownloadManager(message, data)
	if err != nil {
		return err
	}

	filePath, err := Download(dm, message.File)
	if err != nil {
		return err
	}

	task := analyzer.Task{
		LocalFile: filePath,
		FileID:    message.File,
		MaxIssues: int(message.Unix),
	}

	if err := d.Analyzer.Analyze(task, d.Writer); err != nil {
		return err
	}

	return os.Remove(task.LocalFile)
}
