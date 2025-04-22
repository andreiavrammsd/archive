package endpoint

import (
	"net/http"

	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
	s "github.com/andreiavrammsd/async-job-runner-cluster/service"
)

type Sync struct {
	JS s.Job
}

type SyncJobArgs m.Job
type SyncJobResult bool

type SyncStartJobArgs SyncJobArgs
type SyncStartJobResult SyncJobResult

func (s *Sync) JobStart(r *http.Request, args *SyncStartJobArgs, result *SyncStartJobResult) error {
	job := m.Job{}
	job.ID = args.ID
	job.Name = args.Name
	job.Payload = args.Payload
	job.WorkerID = args.WorkerID
	job.Time = args.Time
	s.JS.Store(job)

	*result = true
	return nil
}

type SyncEndJobArgs SyncJobArgs
type SyncEndJobResult SyncJobResult

func (s *Sync) JobEnd(r *http.Request, args *SyncEndJobArgs, result *SyncEndJobResult) error {
	job := m.Job{}
	job.ID = args.ID
	s.JS.Delete(job)

	*result = true
	return nil
}
