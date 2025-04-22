package app

import (
	"github.com/andreiavrammsd/async-job-runner-cluster/jobs"
)

var JobsDefinition = jobs.Definition{
	"Email": jobs.DefinitionEntry{
		Job: new(jobs.Email),
		Payload: new(jobs.EmailPayload),
	},
	"Cleanup": jobs.DefinitionEntry{
		Job: new(jobs.Cleanup),
		Payload: new(jobs.CleanupPayload),
	},
}
