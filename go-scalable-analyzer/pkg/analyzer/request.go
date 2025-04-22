package analyzer

import (
	"encoding/json"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/downloadmanager"
)

type Request struct {
	Provider  downloadmanager.Provider
	Username  string
	File      string
	Time      int64
	Status    string
	AnalyzeID interface{} `bson:"analyze_id,omitempty"`
}

func (c *Request) Encode() ([]byte, error) {
	return json.Marshal(c)
}

func (c *Request) Decode(data []byte) error {
	return json.Unmarshal(data, c)
}
