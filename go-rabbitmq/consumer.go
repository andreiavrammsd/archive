package rabbitmq

import "github.com/streadway/amqp"

// ConsumerConfig holds parameters for queue consume operation
type ConsumerConfig struct {
	Consumer  string
	AutoAck   bool
	Exclusive bool
	NoLocal   bool
	NoWait    bool
	Args      amqp.Table
	Callback  Consumer
}

// Consumer is the type of function to handle messages from a queue
type Consumer func(delivery *Delivery)

// Delivery holds each message read from a queue
type Delivery struct {
	amqp.Delivery
}
