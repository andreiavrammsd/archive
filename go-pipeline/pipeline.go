package main

import (
	"fmt"
	"log"
)

type Pipeline struct {
	workers []Worker
}

func (pe *Pipeline) Execute(u User) {
	var err error
	for _, w := range pe.workers {
		// If one worker returns an error, no need to continue
		if err != nil {
			log.Println(fmt.Sprintf("Error: %s", err.Error()), u)
			break
		}

		u, err = w.Work(u)
	}
}
