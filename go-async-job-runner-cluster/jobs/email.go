package jobs

import (
	"time"
	"log"
)

type Email struct {
}

type EmailPayload struct {
	Email string `json:"email"`
	Subject  string `json:"subject"`
}

func (e *Email) Run(payload interface{}) error {
	email := payload.(*EmailPayload)
	log.Println("Sending mail... ", email.Email, email.Subject)
	time.Sleep(time.Second * 15)
	return nil
}

