package main

import (
	"errors"
	"strings"
)

type Worker interface {
	Work(u User) (User, error)
}

// UniqueInsurer will keep users in memory and return only new ones
type UniqueInsurer struct {
	memory []*User
}

func (ui *UniqueInsurer) Work(u User) (User, error) {
	for _, m := range ui.memory {
		if u.ID == m.ID {
			return u, errors.New("User exists in memory")
		}
	}

	ui.memory = append(ui.memory, &u)

	return u, nil
}

// Capitalizer will capitalize last name
type Capitalizer struct {
}

func (c Capitalizer) Work(u User) (User, error) {
	u.LastName = strings.ToUpper(u.LastName)
	return u, nil
}

// BoundariesApplier truncates names that are too long
type BoundariesApplier struct {
	MaxLength int
}

func (ba BoundariesApplier) Work(u User) (User, error) {
	u.FirstName = ba.truncate(u.FirstName)
	u.LastName = ba.truncate(u.LastName)
	return u, nil
}

func (ba BoundariesApplier) truncate(text string) string {
	if len(text) > ba.MaxLength {
		text = text[0:ba.MaxLength]
	}
	
	return text
}

type Emitter struct {
	ch chan User
}

func (e Emitter) Work(u User) (User, error) {
	e.ch <- u
	return u, nil
}
