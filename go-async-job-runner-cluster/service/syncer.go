package service

import (
	"github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type Syncer interface {
	SyncWorker()
}
type SyncerConfig struct {
	Port    int
	Path    string
	Workers []model.Worker
}

type syncer struct {
	db     Database
	config SyncerConfig
}

func (s *syncer) SyncWorker() {
	for _, worker := range s.config.Workers {
		worker.Port = s.config.Port
		worker.Path = s.config.Path
		s.db.Store(worker.ID, worker)
	}
}

func NewSyncService(db Database, config SyncerConfig) Syncer {
	return &syncer{db, config}
}
