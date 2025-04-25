package analyzer

import (
	"encoding/json"
	"log"
	"time"

	"github.com/andreiavrammsd/tasks-runner/pkg/queue"
)

type Task struct {
	LocalFile string
	FileID    string
	MaxIssues int
}

func (t *Task) Encode() ([]byte, error) {
	return json.Marshal(t)
}

func (t *Task) Decode(data []byte) error {
	return json.Unmarshal(data, t)
}

type Issue struct {
	Type        int
	Description string
	CreatedAt   time.Time `bson:"created_at"`
}

func (t *Issue) Encode() ([]byte, error) {
	return json.Marshal(t)
}

func (t *Issue) Decode(data []byte) error {
	return json.Unmarshal(data, t)
}

type Analyzer struct {
}

func New() *Analyzer {
	return &Analyzer{}
}

func (s *Analyzer) Analyze(task Task, writer queue.Writer) (err error) {
	log.Printf("Analyzing file: %s...", task.LocalFile)
	return nil
}
