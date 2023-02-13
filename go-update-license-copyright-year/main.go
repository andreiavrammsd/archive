package main

import "fmt"

func main() {
	for {
		fmt.Print(labels.Start)

		config.Username = readInput(labels.Username, "", true)

	Loop:
		for {
			option := readInput(labels.SelectAction, options.List, false)

			switch option {
			case options.Update:
				UpdateAction(config)
			case options.Restart:
				githubAccessToken = ""
				break Loop
			default:
				ListAction(config)
			}
		}
	}
}
