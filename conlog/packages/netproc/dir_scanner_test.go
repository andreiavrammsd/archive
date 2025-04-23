package netproc

import (
	"errors"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewProcessDirScanner(t *testing.T) {
	pds := NewProcessDirScanner()
	assert.IsType(t, &processDirScanner{}, pds)
}

func TestProcessDirScanner_Scan(t *testing.T) {
	pds := NewProcessDirScanner()

	expected := map[string]string{
		"/proc/10276/fd/316": "socket:[12913042]",
	}

	filepathGlob = func(pattern string) (matches []string, err error) {
		return []string{"/proc/10276/fd/316"}, nil
	}

	osReadlink = func(name string) (string, error) {
		return "socket:[12913042]", nil
	}

	result, err := pds.Scan()
	assert.Equal(t, expected, result)
	assert.Nil(t, err)
}

func TestProcessDirScanner_ScanWhenFilepathGlobFails(t *testing.T) {
	pds := NewProcessDirScanner()

	expected := map[string]string{}

	filepathGlob = func(pattern string) (matches []string, err error) {
		return []string{}, errors.New("")
	}

	result, err := pds.Scan()
	assert.Equal(t, expected, result)
	assert.NotNil(t, err)
}

func TestProcessDirScanner_ScanWhenOsReadlinkFails(t *testing.T) {
	pds := NewProcessDirScanner()

	expected := map[string]string{}

	filepathGlob = func(pattern string) (matches []string, err error) {
		return []string{"/proc/10276/fd/316"}, nil
	}

	osReadlink = func(name string) (string, error) {
		return "", errors.New("")
	}

	result, err := pds.Scan()
	assert.Equal(t, expected, result)
	assert.Nil(t, err)
}
