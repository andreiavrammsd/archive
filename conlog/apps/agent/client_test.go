package main

import (
	"errors"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewClient(t *testing.T) {
	readFile = func(filename string) ([]byte, error) {
		var result string
		if filename == "/etc/hostname" {
			result = "hostname"
		} else if filename == "/etc/machine-id" {
			result = "a45b7f521c264992928a4d02a2c1056f"
		} else {
			result = "Linux version 4.10.0-42-generic"
		}
		return []byte(result), nil
	}

	validatorValidate = func(v interface{}) error {
		return nil
	}

	id := "uuid"
	secret := "secret"
	hostname := "hostname"
	uuid := "a45b7f521c264992928a4d02a2c1056f"
	uname := "Linux version 4.10.0-42-generic"

	config := Config{}
	config.Agent.ClientID = id
	config.Agent.ClientSecret = secret
	client, err := NewClient(config)

	assert.Equal(t, id, client.ID)
	assert.Equal(t, secret, client.Secret)
	assert.Equal(t, hostname, client.getHostname())
	assert.Equal(t, uuid, client.getUUID())
	assert.Equal(t, uname, client.getUname())
	assert.Nil(t, err)
}

func TestNewClientWhenValidationFails(t *testing.T) {
	readFile = func(filename string) ([]byte, error) {
		return []byte(""), nil
	}

	validatorValidate = func(v interface{}) error {
		return errors.New("")
	}

	config := Config{}
	_, err := NewClient(config)

	assert.NotNil(t, err)
}
