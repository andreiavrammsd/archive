package endpoint

import (
	"net/http"

	s "github.com/andreiavrammsd/async-job-runner-cluster/service"
)

type Cluster struct {
	WS s.Worker
	JS s.Job
}

type ClusterStateArgs struct{}
type ClusterStateResult struct {
	Workers int `json:"workers"`
	Jobs    int `json:"jobs"`
}

func (c *Cluster) State(r *http.Request, args *ClusterStateArgs, result *ClusterStateResult) error {
	state := ClusterStateResult{
		c.WS.CountAll(),
		c.JS.CountAll(),
	}

	*result = ClusterStateResult(state)
	return nil
}
