package main

import (
	"net/http"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestGetHttpClientWithToken(t *testing.T) {
	client := getHTTPClient("token")
	assert.IsType(t, client, &http.Client{})
}

func TestGetHttpClientWithoutToken(t *testing.T) {
	client := getHTTPClient("")
	assert.IsType(t, client, &http.Client{})
}
