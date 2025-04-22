package tasks

import (
	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/logger"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/storage"
)

type SendDownloadRequest struct {
	ConcurrencyCount uint
	Input            chan analyzer.Request
	DBWriter         storage.Writer
	QueueWriter      queue.Writer
	Logger           logger.Logger
}

func (d *SendDownloadRequest) Concurrency() uint {
	return d.ConcurrencyCount
}

func (d *SendDownloadRequest) Run() error {
	for {
		req := <-d.Input

		req.Status = "QUEUED"

		id, err := d.DBWriter.Write(req)
		if err != nil {
			d.Logger.Error(err)
			continue
		}

		req.AnalyzeID = id

		if err := d.QueueWriter.Write(&req); err != nil {
			d.Logger.Error(err)
		}
	}
}
