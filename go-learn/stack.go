package main

import (
	"errors"
	"fmt"
	"log"
)

type stack struct {
	items   []int
	maxSize int
	top     int
}

func (s *stack) push(i int) error {
	if s.top == s.maxSize {
		return errors.New("stack is full")
	}

	s.items = append(s.items, i)
	s.top++

	return nil
}

func (s *stack) pop() (int, error) {
	if s.top == 0 {
		return 0, errors.New("stack is empty")
	}

	s.top--
	i := s.items[s.top]
	s.items = s.items[:s.top]

	return i, nil
}

func (s *stack) peek() int {
	return s.items[s.top]
}

func (s *stack) empty() {
	s.items = nil
}

func listStack(s *stack) {
	for i := 0; i < len(s.items); i++ {
		fmt.Printf("%d ", s.items[i])
	}
	fmt.Println()
}

func main() {
	s := &stack{make([]int, 0), 3, 0}

	if err := s.push(5); err != nil {
		log.Fatal(err)
	}
	if err := s.push(9); err != nil {
		log.Fatal(err)
	}
	if err := s.push(6); err != nil {
		log.Fatal(err)
	}

	fmt.Print("list: ")
	listStack(s)

	fmt.Print("pop: ")
	fmt.Println(s.pop())
	fmt.Print("pop: ")
	fmt.Println(s.pop())

	fmt.Print("list: ")
	listStack(s)

	s.empty()
	fmt.Print("list: ")
	listStack(s)
}
