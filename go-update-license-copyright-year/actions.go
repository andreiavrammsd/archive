package main

import (
	"fmt"

	"github.com/fatih/color"
)

// ListAction lists paginated repositories
func ListAction(config *Config) {
	client := NewGithubClient(config)
	page := 1

Loop:
	for {
		repos, resp, err := client.GetRepositories(page)

		if err != nil {
			fmt.Println(err.Error())
			return
		}

		for _, repo := range repos {
			fmt.Printf("\n%s", *repo.Name)
		}

		if resp.NextPage == 0 {
			break
		}

		option := readInput(labels.NextPageOrSkip, options.NextPage, false)
		switch option {
		case options.Skip:
			break Loop
		default:
			page++
		}
	}

	fmt.Println()
}

var githubAccessToken string

// UpdateAction brings copyright year up to date if license files found
func UpdateAction(config *Config) {
	if len(githubAccessToken) == 0 {
		githubAccessToken = readInput(labels.Token, config.Token, true)
		config.Token = githubAccessToken
	}
	config.LicenseFilenames = readInput(labels.LicenseFilenames, config.LicenseFilenames, false)
	config.Branch = readInput(labels.Branch, config.Branch, false)
	config.CommitMessage = readInput(labels.CommitMessage, config.CommitMessage, false)
	config.CurrentYear = readInput(labels.CurrentYear, config.CurrentYear, false)

	client := NewGithubClient(config)
	page := 1

	for {
		repos, resp, err := client.GetRepositories(page)

		if err != nil {
			fmt.Println(err.Error())
			continue
		}

		for _, repo := range repos {
			fmt.Printf(labels.RepositoryLine, *repo.Name)

			file, err := client.GetFileContent(repo.Name)
			if err != nil {
				color.Red(err.Error())
				continue
			}

			oldContent, err := file.GetContent()
			if err != nil {
				color.Red(err.Error())
				continue
			}

			newContent, err := updateCopyrightYear(oldContent, config.CopyrightPattern, config.CurrentYear)
			if err != nil {
				color.Red(err.Error())
				continue
			}

			err = client.UpdateFile(repo, file, &newContent)
			if err != nil {
				color.Red(err.Error())
				continue
			}

			if len(newContent) > 0 {
				color.Green("Updated")
			}
			fmt.Println(newContent)
			break
		}

		if resp.NextPage == 0 {
			break
		}
	}

	fmt.Println()
}
