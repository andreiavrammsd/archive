package main

import (
	"context"
	"net/http"

	"golang.org/x/oauth2"
)

func getHTTPClient(token string) *http.Client {
	var ts oauth2.TokenSource
	if len(token) > 0 {
		ts = oauth2.StaticTokenSource(
			&oauth2.Token{
				AccessToken: token,
			},
		)
	}

	return oauth2.NewClient(context.TODO(), ts)
}
