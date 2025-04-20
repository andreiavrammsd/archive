package main

import (
	"errors"
	"fmt"
	"log"
)

type queue struct {
	items   []int
	maxSize int
	tail    int
}

func (s *queue) push(i int) error {
	if s.tail == s.maxSize {
		return errors.New("queue is full")
	}

	s.items = append(s.items, i)
	s.tail++

	return nil
}

func (s *queue) pop() (int, error) {
	if s.tail == 0 {
		return 0, errors.New("queue is empty")
	}

	s.tail--
	i := s.items[0]
	s.items = s.items[1:]

	return i, nil
}

func listQueue(s *queue) {
	for i := 0; i < len(s.items); i++ {
		fmt.Printf("%d ", s.items[i])
	}
	fmt.Println()
}

func main() {
	q := &queue{make([]int, 0), 4, 0}

	if err := q.push(5); err != nil {
		log.Fatal(err)
	}
	if err := q.push(9); err != nil {
		log.Fatal(err)
	}
	if err := q.push(6); err != nil {
		log.Fatal(err)
	}
	if err := q.push(12); err != nil {
		log.Fatal(err)
	}

	fmt.Print("list: ")
	listQueue(q)

	fmt.Print("pop: ")
	fmt.Println(q.pop())
	fmt.Print("pop: ")
	fmt.Println(q.pop())

	fmt.Print("list: ")
	listQueue(q)
}
