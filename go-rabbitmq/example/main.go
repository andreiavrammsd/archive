package main

import (
	"encoding/json"
	"fmt"
	"log"
	"time"

	"github.com/andreiavrammsd/go-rabbitmq"
)

// Email message
type Email struct {
	From    string `json:"from"`
	To      string `json:"to"`
	Message string `json:"message"`
}

var (
	conn      *rabbitmq.Connection
	queueName = "emails"
	config    = &rabbitmq.Config{
		Scheme:   "amqp",
		Host:     "127.0.0.1",
		Port:     5672,
		Username: "guest",
		Password: "guest",
		Vhost:    "/",
	}
	consumer rabbitmq.Consumer
	err      error
)

func main() {
	// Establish connection to Rabbit server
	conn, err = rabbitmq.New(config)
	checkError(err)

	// Open a channel
	ch, err := conn.Channel()
	checkError(err)

	// Declare a queue
	q, err := ch.Queue(queueName)
	checkError(err)

	// Send messages to queue
	go func() {
		t := time.Tick(time.Millisecond * 100)
		for {
			<-t

			email := Email{
				From:    "myaddress@myprovider.tld",
				To:      "youraddress@yourprovider.tld",
				Message: "Hello, GO @" + time.Now().Format("15:04:05"),
			}

			message, err := json.Marshal(email)
			checkError(err)

			err = q.Publish(message)
			checkError(err)
		}
	}()

	// Define a consumer function where we'll get all the messages from the queue
	consumer = func(delivery *rabbitmq.Delivery) {
		email := Email{}
		err := json.Unmarshal(delivery.Body, &email)
		checkError(err)
		fmt.Println(email)

		// If everything ok, acknowledge message to server
		err = delivery.Ack(false)
		checkError(err)

	}

	// Run one or more consumers on different threads
	config := &rabbitmq.ConsumerConfig{
		Callback: consumer,
	}
	go consume(config)
	go consume(config)

	// Run forever
	forever := make(chan bool)
	<-forever
}

func consume(config *rabbitmq.ConsumerConfig) {
	// We keep a single connection to rabbit server, but we open a channel for
	// each thread we're consuming the queue from
	ch, err := conn.Channel()
	checkError(err)

	// Declare the queue on the new channel
	q, err := ch.Queue(queueName)
	checkError(err)

	// Pass our consumer function to the queue
	err = q.Consume(config)
	checkError(err)
}

func checkError(err error) {
	if err != nil {
		log.Fatal(err)
	}
}
