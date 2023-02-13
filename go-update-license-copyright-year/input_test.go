package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestReadInputDefaultValue(t *testing.T) {
	scan = func() string {
		return " "
	}
	defaultValue := "default"
	value := readInput("text", defaultValue, false)

	assert.Equal(t, defaultValue, value)
}

func TestReadInput(t *testing.T) {
	expected := "value"
	scan = func() string {
		return expected
	}
	value := readInput("text", "", false)

	assert.Equal(t, expected, value)
}

func TestReadInputRequired(t *testing.T) {
	expected := "value"
	scan = func() string {
		return expected
	}
	value := readInput("text", "", true)

	assert.Equal(t, expected, value)
}
