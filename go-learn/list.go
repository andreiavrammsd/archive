package main

type node struct {
	value interface{}
	next  *node
}

func main() {
	n1 := &node{1, nil}
	n2 := &node{2, n1}
	n3 := &node{3, n2}

	n := n3
	for n3.next != nil {

	}
}
