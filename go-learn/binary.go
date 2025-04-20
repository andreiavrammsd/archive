package main

import (
	"fmt"
	"sort"
)

func search(items []int, val int) int {
	first := 0
	last := len(items) - 1

	for first < last {
		mid := (first + last) / 2

		if items[mid] == val {
			return mid
		}

		if items[mid] < val {
			first = mid + 1
		} else {
			last = mid - 1
		}
	}

	return -1
}

func main() {
	items := []int{2, 40, 9, 4, 5, 6}
	fmt.Println(items)
	sort.Ints(items)
	fmt.Println(items)

	fmt.Println(search(items, 9))
	fmt.Println(search(items, 99))
}
