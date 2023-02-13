package main

import (
	"fmt"
	"strconv"
	"time"
)

// Config params
type Config struct {
	Token            string
	Username         string
	LicenseFilenames string
	Branch           string
	CopyrightPattern string
	CommitMessage    string
	CurrentYear      string
	Repository       *RepositoryConfig
}

// RepositoryConfig for listing
type RepositoryConfig struct {
	ListPerPage      int
	ListSortBy       string
	ListFilterByType string
}

// Labels for prompt
type Labels struct {
	Start                    string
	Token                    string
	Username                 string
	Required                 string
	SelectAction             string
	LicenseFilenames         string
	Branch                   string
	CommitMessage            string
	CurrentYear              string
	NextPageOrSkip           string
	RepositoryLine           string
	LicenseFileNotFound      string
	CopyrightPatternNotFound string
	YearHasNotChanged        string
}

// Options values for user input
type Options struct {
	List     string
	Update   string
	Restart  string
	NextPage string
	Skip     string
}

var config = &Config{
	LicenseFilenames: "LICENSE, LICENSE.md, LICENSE.txt",
	Branch:           "master",
	CopyrightPattern: "Copyright \\(c\\) ([0-9]{4})-?([0-9]{4})?",
	CommitMessage:    "Update license.",
	CurrentYear:      strconv.FormatInt(int64(time.Now().Year()), 10),
	Repository: &RepositoryConfig{
		ListPerPage:      30,
		ListSortBy:       "name",
		ListFilterByType: "owner",
	},
}

var options = &Options{
	List:     "l",
	Update:   "u",
	Restart:  "r",
	NextPage: "n",
	Skip:     "s",
}

var labels = &Labels{
	Start:    "\nQuit any time with Ctrl + C\n\n",
	Token:    "\nGithub access token: ",
	Username: "Insert username: ",
	Required: "Required\n",
	SelectAction: fmt.Sprintf(
		"\nSelect your action: List repositories (%s = default) | Update license (%s) | Restart (%s): ",
		options.List,
		options.Update,
		options.Restart,
	),
	LicenseFilenames:         fmt.Sprintf("License filenames, comma separated (%s): ", config.LicenseFilenames),
	Branch:                   fmt.Sprintf("Branch (%s): ", config.Branch),
	CommitMessage:            fmt.Sprintf("Commit message (%s): ", config.CommitMessage),
	CurrentYear:              fmt.Sprintf("Current year (%s): ", config.CurrentYear),
	NextPageOrSkip:           fmt.Sprintf("\nNext page (%s) | Skip (%s): ", options.NextPage, options.Skip),
	RepositoryLine:           "\n%s... ",
	LicenseFileNotFound:      "License file not found.",
	CopyrightPatternNotFound: fmt.Sprintf("Copyright pattern not found: %s", config.CopyrightPattern),
	YearHasNotChanged:        "Year has not changed",
}
