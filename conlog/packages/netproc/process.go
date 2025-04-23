package netproc

import "time"

type Process struct {
	Protocol    string `validate:"min=3"`
	UserID      string `validate:"min=1"`
	User        string `validate:"min=1"`
	Name        string `validate:"min=1"`
	PID         int
	Exe         string
	State       string
	IP          string
	Port        int64
	ForeignIP   string
	ForeignPort int64
	Inode       int
	Time        time.Time
}

type Processes map[string]Process
