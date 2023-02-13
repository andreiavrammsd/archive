package main

import (
	"errors"
	"fmt"
	"regexp"
	"strings"
)

func updateCopyrightYear(content string, copyrightPattern string, newYear string) (string, error) {
	re, err := regexp.Compile(copyrightPattern)
	if err != nil {
		return "", err
	}

	copyright := re.FindStringSubmatch(content)
	if len(copyright) == 0 {
		return "", errors.New(labels.CopyrightPatternNotFound)
	}

	lastIndex := len(copyright) - 1
	singleYear := false
	if copyright[lastIndex] == "" {
		copyright = copyright[:lastIndex]
		lastIndex--
		singleYear = true
	}
	oldCopyright := copyright[0]
	oldYear := copyright[lastIndex]

	if newYear == oldYear {
		return "", errors.New(labels.YearHasNotChanged)
	}

	if singleYear {
		newYear = fmt.Sprintf("%s-%s", oldYear, newYear)
	}

	newCopyright := strings.Replace(oldCopyright, oldYear, newYear, 1)
	content = strings.Replace(content, oldCopyright, newCopyright, 1)

	return content, nil
}
