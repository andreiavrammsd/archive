package main

import (
	"errors"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewConfig(t *testing.T) {
	ymlRead = func(file string, out interface{}) error {
		return nil
	}

	_, err := NewConfig("")
	assert.Nil(t, err)
}

func TestNewConfigWhenFileReadFails(t *testing.T) {
	ymlRead = func(file string, out interface{}) error {
		return errors.New("")
	}

	_, err := NewConfig("")
	assert.NotNil(t, err)
}
