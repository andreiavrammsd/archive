package service

import (
	"github.com/gorilla/rpc/json"
	"bytes"
	"net/http"
	"errors"
)

const METHOD = "POST"
const CONTENT_TYPE = "application/json"

type RPCClient interface {
	Call(request ClientRequest, response interface{}) (error)
}

type ClientRequest struct {
	URL          string
	Method       string
	Args         interface{}
}

type client struct {
}

func (c *client) Call(request ClientRequest, response interface{}) (error) {
	message, err := json.EncodeClientRequest(request.Method, request.Args)
	if err != nil {
		return err
	}

	req, err := http.NewRequest(METHOD, request.URL, bytes.NewBuffer(message))
	if err != nil {
		return err
	}
	req.Header.Set("Content-Type", CONTENT_TYPE)

	client := new(http.Client)
	resp, err := client.Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	err = json.DecodeClientResponse(resp.Body, response)
	if err != nil {
		var b []byte
		resp.Body.Read(b)
		return errors.New(string(b))
	}

	return nil
}

func NewRPCClient() RPCClient {
	return &client{}
}
