package analyzer

import (
	"encoding/json"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue"
)

type Task struct {
	LocalFile string
	FileID    string
	MaxIssues int
	AnalyzeID interface{}
}

func (t *Task) Encode() ([]byte, error) {
	return json.Marshal(t)
}

func (t *Task) Decode(data []byte) error {
	return json.Unmarshal(data, t)
}

type Analyzer struct {
	writer queue.Writer
}

func New(writer queue.Writer) *Analyzer {
	return &Analyzer{
		writer: writer,
	}
}

func (s *Analyzer) Analyze(task Task) (err error) {
	return nil
}
