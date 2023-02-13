package rabbitmq

import (
	"github.com/streadway/amqp"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestNewSuccess(t *testing.T) {
	dial = func(url string) (*amqp.Connection, error) {
		return &amqp.Connection{}, nil
	}

	config := &Config{
		Scheme:   "amqp",
		Host:     "127.0.0.1",
		Port:     5672,
		Username: "guest",
		Password: "guest",
		Vhost:    "/",
	}
	conn, err := New(config)

	expectedConnection := &Connection{
		Config:     config,
		Connection: &amqp.Connection{},
	}
	assert.Equal(t, expectedConnection, conn)
	assert.Nil(t, err)
}

func TestNewFail(t *testing.T) {
	dial = func(url string) (*amqp.Connection, error) {
		return nil, nil
	}

	config := &Config{
		Scheme:   "invalid scheme",
		Host:     "127.0.0.1",
		Port:     5672,
		Username: "guest",
		Password: "guest",
		Vhost:    "/",
	}
	conn, err := New(config)

	expectedConnection := &Connection{
		Config:     config,
		Connection: nil,
	}
	assert.Equal(t, expectedConnection, conn)
	assert.Nil(t, err)
}
