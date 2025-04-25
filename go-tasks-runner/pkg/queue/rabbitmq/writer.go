package rabbitmq

import (
	"github.com/andreiavrammsd/tasks-runner/pkg/queue"

	"github.com/streadway/amqp"
)

type WriterConfig struct {
	Connection *amqp.Connection
	Queue      string
}

type Writer struct {
	config  *WriterConfig
	channel *amqp.Channel
	queue   amqp.Queue
}

func (w *Writer) Write(data queue.EncoderDecoder) error {
	body, err := data.Encode()
	if err != nil {
		return err
	}

	if err := w.channel.Publish(
		"",           // publish to an exchange
		w.queue.Name, // routing to 0 or more queues
		false,        // mandatory
		false,        // immediate
		amqp.Publishing{
			Headers:         amqp.Table{},
			ContentType:     "text/plain",
			ContentEncoding: "",
			Body:            body,
			DeliveryMode:    amqp.Persistent, // 1=non-persistent, 2=persistent
			Priority:        0,               // 0-9
		},
	); err != nil {
		return err
	}

	return nil
}

func NewWriter(config *WriterConfig) (*Writer, error) {
	channel, err := config.Connection.Channel()
	if err != nil {
		return nil, err
	}

	queue, err := channel.QueueDeclare(
		config.Queue,
		true,
		false,
		false,
		false,
		nil,
	)
	if err != nil {
		return nil, err
	}

	return &Writer{
		config:  config,
		channel: channel,
		queue:   queue,
	}, nil
}
