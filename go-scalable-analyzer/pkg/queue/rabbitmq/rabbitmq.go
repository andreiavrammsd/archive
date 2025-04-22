package rabbitmq

import (
	"fmt"

	"github.com/streadway/amqp"
)

// ClientConfig holds configuration values needed to create a client
type ClientConfig struct {
	Host     string
	Username string
	Password string
}

// NewClient creates RabbitMQ client
func NewClient(config ClientConfig) (connection *amqp.Connection, err error) {
	uri := fmt.Sprintf("amqp://%s:%s@%s/", config.Username, config.Password, config.Host)
	connection, err = amqp.Dial(uri)
	return
}
