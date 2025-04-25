package rabbitmq

import (
	"github.com/andreiavrammsd/tasks-runner/pkg/logger"
	"github.com/andreiavrammsd/tasks-runner/pkg/queue"

	"github.com/streadway/amqp"
)

// add batch consume
type ReaderConfig struct {
	Connection *amqp.Connection
	Queue      string
	Logger     logger.Logger
}

type Reader struct {
	config     *ReaderConfig
	connection *amqp.Connection
}

// context to stop reading
func (r *Reader) Read(deliver queue.Delivery) error {
	channel, err := r.connection.Channel()
	if err != nil {
		return err
	}

	q, err := channel.QueueDeclare(
		r.config.Queue,
		true,
		false,
		false,
		false,
		nil,
	)
	if err != nil {
		return err
	}

	deliveries, err := channel.Consume(
		q.Name,
		"",
		false,
		false,
		false,
		false,
		nil,
	)
	if err != nil {
		return err
	}

	for d := range deliveries {
		if err := deliver(d.Body); err != nil {
			r.config.Logger.Error(err)
		} else {
			if err := d.Ack(false); err != nil {
				r.config.Logger.Error(err)
			}
		}
	}

	return nil
}

func NewReader(config *ReaderConfig) (*Reader, error) {
	return &Reader{
		config:     config,
		connection: config.Connection,
	}, nil
}
