package main

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
)

type APIClient struct {
	Addr       string
	HTTPClient *http.Client
}

func (c *APIClient) Send(r analyzer.Request) (err error) {
	data, err := r.Encode()
	if err != nil {
		return
	}

	res, err := c.HTTPClient.Post(c.Addr+"analyze", "application/json", bytes.NewBuffer(data))
	if err != nil {
		return
	}

	defer func() {
		if e := res.Body.Close(); e != nil {
			err = e
		}
	}()

	if res.StatusCode != http.StatusOK {
		return fmt.Errorf("cannot send analyze request. code %d", res.StatusCode)
	}

	_, err = io.Copy(ioutil.Discard, res.Body)

	return
}
