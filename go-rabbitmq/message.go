package rabbitmq

import "github.com/streadway/amqp"

// messageConfig holds the parameters for a queue message
type messageConfig struct {
	Body         []byte
	ContentType  string
	DeliveryMode uint8
	Exchange     string
	RoutingKey   string
	Mandatory    bool
	Immediate    bool
}

// message is a basic message configuration
var message = &messageConfig{
	ContentType:  "text/plain",
	DeliveryMode: amqp.Persistent,
	Exchange:     "",
	RoutingKey:   "",
	Mandatory:    false,
	Immediate:    false,
}
