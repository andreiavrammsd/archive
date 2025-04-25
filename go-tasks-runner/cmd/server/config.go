package main

import (
	"time"

	"github.com/andreiavrammsd/tasks-runner/pkg/queue/rabbitmq"
)

type Mongo struct {
	Host           string
	Username       string
	Password       string
	Database       string
	Collection     string
	ConnectTimeout time.Duration
	QueryTimeout   time.Duration
}

type Config struct {
	Mongo         Mongo
	Rabbit        rabbitmq.ClientConfig
	QueueDownload string
	QueueAnalyze  string
	QueueIssues   string
}
