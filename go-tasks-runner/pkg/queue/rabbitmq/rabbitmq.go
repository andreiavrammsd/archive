package rabbitmq

import (
	"fmt"

	"github.com/streadway/amqp"
)

type ClientConfig struct {
	Host     string
	Username string
	Password string
}

// add ping and reconnect
func NewClient(config ClientConfig) (connection *amqp.Connection, err error) {
	uri := fmt.Sprintf("amqp://%s:%s@%s/", config.Username, config.Password, config.Host)
	connection, err = amqp.Dial(uri)
	return
}
