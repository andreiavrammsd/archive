package app

import (
	"os"

	"github.com/andreiavrammsd/async-job-runner-cluster/jobs"
	m "github.com/andreiavrammsd/async-job-runner-cluster/model"
	"gopkg.in/yaml.v2"
)

type Config struct {
	Workers []m.Worker
	Server  struct {
		Port int
		Path string
	}
	Health struct {
		Delay int
	}
	JobsDefinition jobs.Definition
}

func NewConfig(file string) (Config, error) {
	c := Config{}

	yamlData, err := os.ReadFile(file)
	if err != nil {
		return c, err
	}

	err = yaml.Unmarshal(yamlData, &c)
	if err != nil {
		return c, err
	}

	c.JobsDefinition = JobsDefinition

	return c, nil
}
