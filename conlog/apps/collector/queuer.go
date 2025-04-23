package main

import (
	"log"

	"github.com/andreiavrammsd/conlog/packages/config"
	"github.com/streadway/amqp"
)

func publisher(conn *amqp.Connection, channel chan []byte) {
	// Set channel
	ch, err := conn.Channel()
	if err != nil {
		log.Println(err)
	}
	defer ch.Close()

	// Declare exchange
	err = ch.ExchangeDeclare(
		config.COLLECTOR_EXCHANGE_NAME, // name
		"fanout",                       // type
		true,                           // durable
		false,                          // auto-deleted
		false,                          // internal
		false,                          // no-wait
		nil,                            // arguments
	)
	if err != nil {
		log.Println(err)
	}

	queue, err := ch.QueueDeclare("processes", true, false, false, false, nil)
	if err != nil {
		log.Println(err)
	}

	err = ch.QueueBind(queue.Name, "", config.COLLECTOR_EXCHANGE_NAME, false, nil)
	if err != nil {
		log.Println(err)
	}

	// Publish
	for {
		msg := <-channel

		err = ch.Publish(
			config.COLLECTOR_EXCHANGE_NAME, // exchange
			"",                             // routing key
			false,                          // mandatory
			false,                          // immediate
			amqp.Publishing{
				ContentType:  "text/plain",
				Body:         []byte(msg),
				DeliveryMode: amqp.Persistent,
			})
		log.Println("published")

		if err != nil {
			log.Println(err)
		}
	}
}
