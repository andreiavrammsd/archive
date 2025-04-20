package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

func defaultHandler(w http.ResponseWriter, r *http.Request) {
	body, err := ioutil.ReadAll(r.Body)
	defer r.Body.Close()
	if err != nil {
		panic(err)
	}

	var users Users
	err = json.Unmarshal(body, &users)
	if err != nil {
		panic(err)
	}

	fmt.Println()
	log.Println("Incoming...")
	log.Println(users, "\n")

	// Send received users on incoming channel
	incoming <- users

	w.Header().Set("Content-Type", "application/json")
	w.Write(body)
}
