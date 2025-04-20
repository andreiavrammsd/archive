package main

import (
	"fmt"
	"sort"
)

func main() {
	items := []int{2, 40, 9, 4, 5, 6}
	items2 := items[:]
	items3 := items[:]
	fmt.Println(items)
	sort.Ints(items)
	fmt.Println(items)

	items2 = append(items2, -23)
	items = items2

	for i := 0; i < len(items); i++ {
		for j := i + 1; j < len(items); j++ {
			if items[i] > items[j] {
				items[i], items[j] = items[j], items[i]
			}
		}
	}

	fmt.Println(items)

	items3 = append(items3, -123)
	items = items3

	quickSort(items)

}

func quickSort(ints []int) {

}
