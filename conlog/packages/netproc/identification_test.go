package netproc

import (
	"errors"
	"os"
	"os/user"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewProcessIdentification(t *testing.T) {
	pi := NewProcessIdentification()
	assert.IsType(t, &processIdentification{}, pi)
}

func TestProcessIdentification_Identify(t *testing.T) {
	pi := NewProcessIdentification()

	p := Process{}
	p.Inode = 12913042
	dirs := map[string]string{
		"/proc/10276/fd/316": "socket:[12913042]",
	}

	expectedPID := 10276
	expectedExe := "/usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java"
	expectedName := "Java"
	expectedUsername := "admin"

	osReadLink = func(name string) (string, error) {
		return expectedExe, nil
	}

	userLookupId = func(uid string) (*user.User, error) {
		u := &user.User{}
		u.Username = expectedUsername
		return u, nil
	}

	result := pi.Identify(p, dirs)
	assert.Equal(t, expectedPID, result.PID)
	assert.Equal(t, expectedExe, result.Exe)
	assert.Equal(t, expectedName, result.Name)
	assert.Equal(t, expectedUsername, result.User)
}

func TestProcessIdentification_IdentifyWhenInodePathNotFound(t *testing.T) {
	pi := NewProcessIdentification()

	p := Process{}
	p.Inode = 12913042
	dirs := map[string]string{
		"/proc/10276/fd/316": "socket:[...]",
	}

	expectedPID := -1
	expectedExe := ""
	expectedName := ""
	expectedUsername := ""

	osReadLink = func(name string) (string, error) {
		return "", &os.PathError{}
	}

	userLookupId = func(uid string) (*user.User, error) {
		u := &user.User{}
		u.Username = expectedUsername
		return nil, errors.New("")
	}

	result := pi.Identify(p, dirs)
	assert.Equal(t, expectedPID, result.PID)
	assert.Equal(t, expectedExe, result.Exe)
	assert.Equal(t, expectedName, result.Name)
	assert.Equal(t, expectedUsername, result.User)
}

func TestProcessIdentification_IdentifyWhenOsReadLinkAndUserLookupIdFail(t *testing.T) {
	pi := NewProcessIdentification()

	p := Process{}
	p.Inode = 12913042
	dirs := map[string]string{
		"/proc/10276/fd/316": "socket:[12913042]",
	}

	expectedPID := 10276
	expectedExe := ""
	expectedName := ""
	expectedUsername := ""

	osReadLink = func(name string) (string, error) {
		return "", &os.PathError{}
	}

	userLookupId = func(uid string) (*user.User, error) {
		u := &user.User{}
		u.Username = expectedUsername
		return nil, errors.New("")
	}

	result := pi.Identify(p, dirs)
	assert.Equal(t, expectedPID, result.PID)
	assert.Equal(t, expectedExe, result.Exe)
	assert.Equal(t, expectedName, result.Name)
	assert.Equal(t, expectedUsername, result.User)
}
