// Package config holds global configuration values
package config

// Move all to .env file

const COLLECTOR_URL = "collector:50051"
const COLLECTOR_SERVER_ADDRESS = ":50051"
const COLLECTOR_EXCHANGE_NAME = "collector"
const COLLECTOR_QUEUE_PUBLISHER_COUNT = 3

const QUEUER_USERNAME = "rbtmqusr"
const QUEUER_PASSWORD = "rbtmqpass"
const QUEUER_HOST = "queuer"
const QUEUER_PORT = 5672
