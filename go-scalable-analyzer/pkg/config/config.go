package config

import (
	"fmt"
	"os"
	"strconv"
	"time"

	"github.com/joho/godotenv"
)

// Config keeps the configuration values
type Config struct {
	API struct {
		ServerAddress string
		ClientAddress string
	}
	Rabbit struct {
		Host     string
		Username string
		Password string
	}
	Mongo struct {
		Host           string
		Username       string
		Password       string
		ConnectTimeout time.Duration
		QueryTimeout   time.Duration
	}
	QueueDownload          string
	QueueAnalyze              string
	QueueIssues           string
	DatabaseIssues        string
	CollectionIssues      string
	CollectionAnalyzeRequests string
}

// New creates Config from environment. Environment files can be passed.
func New(filenames ...string) (Config, error) {
	if err := godotenv.Load(filenames...); err != nil {
		return Config{}, err
	}

	conf := Config{}

	conf.API.ServerAddress = os.Getenv("API_SERVER_ADDRESS")
	conf.API.ClientAddress = os.Getenv("API_CLIENT_ADDRESS")

	conf.Rabbit.Host = fmt.Sprintf("%s:%s", os.Getenv("RABBITMQ_HOST"), os.Getenv("RABBITMQ_PORT"))
	conf.Rabbit.Username = os.Getenv("RABBITMQ_DEFAULT_USER")
	conf.Rabbit.Password = os.Getenv("RABBITMQ_DEFAULT_PASS")

	conf.Mongo.Host = fmt.Sprintf("%s:%s", os.Getenv("MONGO_HOST"), os.Getenv("MONGO_PORT"))
	conf.Mongo.Username = os.Getenv("MONGO_INITDB_ROOT_USERNAME")
	conf.Mongo.Password = os.Getenv("MONGO_INITDB_ROOT_PASSWORD")

	duration, err := strconv.ParseUint(os.Getenv("MONGO_CONNECT_TIMEOUT_SECONDS"), 10, 0)
	if err != nil {
		return Config{}, err
	}
	conf.Mongo.ConnectTimeout = time.Second * time.Duration(duration)

	duration, err = strconv.ParseUint(os.Getenv("MONGO_QUERY_TIMEOUT_SECONDS"), 10, 0)
	if err != nil {
		return Config{}, err
	}
	conf.Mongo.QueryTimeout = time.Second * time.Duration(duration)

	conf.QueueDownload = os.Getenv("QUEUE_DOWNLOAD")
	conf.QueueAnalyze = os.Getenv("QUEUE_ANALYZE")
	conf.QueueIssues = os.Getenv("QUEUE_ISSUES")

	conf.DatabaseIssues = os.Getenv("DATABASE_ISSUES")
	conf.CollectionIssues = os.Getenv("COLLECTION_ISSUES")
	conf.CollectionAnalyzeRequests = os.Getenv("COLLECTION_ANALYZE_REQUEST")

	return conf, nil
}
