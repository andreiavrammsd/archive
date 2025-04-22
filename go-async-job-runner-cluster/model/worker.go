package model

import "fmt"

type Worker struct {
	ID        uint64 `json:"id"`
	Address   string `json:"address"`
	Port      int `json:"port"`
	Path      string `json:"path"`
	JobsCount int `json:"jobs_count"`
	Error     bool `json:"error"`
}

func (w *Worker) GetURL() string {
	return fmt.Sprintf("http://%s:%d%s", w.Address, w.Port, w.Path)
}
