package main

import (
	"os"

	"gopkg.in/validator.v2"
)

type Client struct {
	ID       string
	Secret   string
	Hostname string `validate:"min=1"`
	UUID     string `validate:"min=20"`
	Uname    string `validate:"min=1"`
}

func (c *Client) getHostname() string {
	files := []string{
		"/etc/hostname",
	}
	return getFirstFileContent(files)
}

func (c *Client) getUUID() string {
	files := []string{
		"/etc/machine-id",
		"/var/lib/dbus/machine-id",
	}
	return getFirstFileContent(files)
}

func (c *Client) getUname() string {
	files := []string{
		"/proc/version",
	}

	return getFirstFileContent(files)
}

var validatorValidate = validator.Validate
var readFile = os.ReadFile

func getFirstFileContent(files []string) string {
	var content []byte
	var err error
	for _, file := range files {
		content, err = readFile(file)
		if err == nil {
			break
		}
	}

	return string(content)
}

func NewClient(config Config) (Client, error) {
	client := Client{}
	client.ID = config.Agent.ClientID
	client.Secret = config.Agent.ClientSecret
	client.Hostname = client.getHostname()
	client.UUID = client.getUUID()
	client.Uname = client.getUname()

	if errs := validatorValidate(client); errs != nil {
		return client, errs
	}

	return client, nil
}
