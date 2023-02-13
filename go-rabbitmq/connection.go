package rabbitmq

import "github.com/streadway/amqp"

// Config holds connection parameters
type Config struct {
	Scheme   string
	Host     string
	Port     int
	Username string
	Password string
	Vhost    string
}

// Connection wrapper to RabbitMQ
type Connection struct {
	Config     *Config
	Connection *amqp.Connection
}

// Channel wrapper
type Channel struct {
	Channel *amqp.Channel
}

var dial = amqp.Dial

// New creates a connection
func New(config *Config) (*Connection, error) {
	uri := amqp.URI{
		Scheme:   config.Scheme,
		Host:     config.Host,
		Port:     config.Port,
		Username: config.Username,
		Password: config.Password,
		Vhost:    config.Vhost,
	}
	conn, err := dial(uri.String())

	q := &Connection{
		Config:     config,
		Connection: conn,
	}

	return q, err
}

// Channel opens a new channel
func (c *Connection) Channel() (*Channel, error) {
	newChannel, err := c.Connection.Channel()
	ch := &Channel{
		Channel: newChannel,
	}

	return ch, err
}
