package analyzer

import (
	"encoding/json"
	"time"
)

type Issue struct {
	IssueTypeID      int
	MatchedText       string
	IssueDescription string
	CreatedAt         time.Time   `bson:"created_at"`
	AnalyzeID            interface{} `bson:"analyze_id"`
}

func (t *Issue) Encode() ([]byte, error) {
	return json.Marshal(t)
}

func (t *Issue) Decode(data []byte) error {
	return json.Unmarshal(data, t)
}
