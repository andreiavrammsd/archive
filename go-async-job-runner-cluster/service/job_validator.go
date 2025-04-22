package service

import (
	"encoding/json"
	"fmt"

	"github.com/andreiavrammsd/async-job-runner-cluster/jobs"
	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
)

type Validator interface {
	Validate(model interface{}) error
}

type jobValidator struct {
	def jobs.Definition
}

func (jv *jobValidator) Validate(model interface{}) error {
	job := model.(m.Job)
	def, ok := jv.def[job.Name]

	if !ok {
		return fmt.Errorf("Job %s not found", job.Name)
	}

	payload := def.Payload
	return json.Unmarshal(job.Payload, &payload)
}

func NewJobValidatorService(def jobs.Definition) Validator {
	return &jobValidator{def}
}
