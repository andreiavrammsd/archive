package main

import (
	"context"
	"errors"
	"strings"

	"github.com/google/go-github/github"
)

// GithubClient to manage repositories
type GithubClient struct {
	Config *Config
	Client *github.Client
	Ctx    context.Context
}

// GetRepositories retrieves repositories by page
func (c *GithubClient) GetRepositories(page int) ([]*github.Repository, *github.Response, error) {
	options := &github.RepositoryListOptions{
		Sort: c.Config.Repository.ListSortBy,
		Type: c.Config.Repository.ListFilterByType,
		ListOptions: github.ListOptions{
			Page:    page,
			PerPage: c.Config.Repository.ListPerPage,
		},
	}

	return c.Client.Repositories.List(c.Ctx, c.Config.Username, options)
}

// GetFileContent retrieves license files content
func (c *GithubClient) GetFileContent(repo *string) (*github.RepositoryContent, error) {
	files := strings.Split(c.Config.LicenseFilenames, ",")

	for _, file := range files {
		fileContent, _, _, err := c.Client.Repositories.GetContents(
			c.Ctx,
			c.Config.Username,
			*repo,
			strings.Trim(file, " "),
			&github.RepositoryContentGetOptions{
				Ref: c.Config.Branch,
			},
		)

		if err == nil {
			return fileContent, nil
		}
	}

	return nil, errors.New(labels.Branch)
}

// UpdateFile writes new license year
func (c *GithubClient) UpdateFile(repo *github.Repository, file *github.RepositoryContent, content *string) error {
	_, _, err := c.Client.Repositories.UpdateFile(
		c.Ctx,
		config.Username,
		*repo.Name,
		*file.Name,
		&github.RepositoryContentFileOptions{
			Message: &config.CommitMessage,
			Content: []byte(*content),
			SHA:     file.SHA,
			Branch:  &config.Branch,
		},
	)

	return err
}

// NewGithubClient instance
func NewGithubClient(config *Config) *GithubClient {
	http := getHTTPClient(config.Token)

	return &GithubClient{
		Config: config,
		Client: github.NewClient(http),
		Ctx:    context.TODO(),
	}
}
