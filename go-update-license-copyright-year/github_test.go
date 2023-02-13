package main

import (
	"testing"

	"github.com/google/go-github/github"
	"github.com/stretchr/testify/assert"
)

func TestNewGithubClientWithToken(t *testing.T) {
	config := &Config{
		Token: "token",
	}
	client := NewGithubClient(config)
	assert.IsType(t, client, &GithubClient{})
	assert.IsType(t, client.Client, &github.Client{})
	assert.Equal(t, config, client.Config)
}

func TestNewGithubClientWithoutToken(t *testing.T) {
	config = &Config{}
	client := NewGithubClient(config)
	assert.IsType(t, client, &GithubClient{})
	assert.IsType(t, client.Client, &github.Client{})
	assert.Equal(t, config, client.Config)
}
