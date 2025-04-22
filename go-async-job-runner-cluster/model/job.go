package model

import (
	"time"
	"encoding/json"
)

type Job struct {
	ID      uint64  `json:"id"`
	Name    string `json:"name"`
	Payload json.RawMessage `json:"payload"`
	WorkerID uint64 `json:"worker"`
	Time    time.Time `json:"time"`
}
