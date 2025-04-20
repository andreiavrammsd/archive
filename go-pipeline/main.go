package main

import (
	"log"
	"net/http"
)

var incoming chan Users
var outgoing chan User

func main() {
	// Incoming and outgoing users channels
	incoming = make(chan Users)
	outgoing = make(chan User)

	// Setup workers and pipeline
	workers := []Worker{
		&UniqueInsurer{
			make([]*User, 0),
		},
		Capitalizer{},
		BoundariesApplier{
			12,
		},
		Emitter{
			outgoing,
		},
	}
	pipeline := Pipeline{
		workers,
	}

	// Pipeline execution runs on a routine and waits for incoming users
	go func(incoming chan Users, pipeline Pipeline) {
		for {
			users := <-incoming
			for _, u := range users {
				pipeline.Execute(u)
			}
		}
	}(incoming, pipeline)

	// This routine waits for users from the Emitter worker
	// and sends them further (now it just prints them)
	go func(outgoing chan User) {
		for {
			u := <-outgoing
			log.Println("Success: Outgoing", u)
		}
	}(outgoing)

	// HTTP server to receive users and send them on the incoming channel
	// to be processed by the pipeline executor
	http.HandleFunc("/", defaultHandler)
	http.ListenAndServe(":8080", nil)
}
