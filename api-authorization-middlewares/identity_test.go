package main

import (
	"errors"
	"testing"

	"github.com/labstack/echo"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
)

type contextMock struct {
	mock.Mock
	echo.Context
}

func (c *contextMock) Set(key string, val interface{}) {
	c.Called(key, val)
}

func (c *contextMock) Get(key string) interface{} {
	args := c.Called(key)
	return args.Get(0)
}

func TestIdentity(t *testing.T) {
	id := &Identity{
		Username:    "username",
		Permissions: CanManageUsers,
		Type:        ModeratorAccount,
	}

	c := &contextMock{}
	c.On("Set", identityKey, id).Once()
	c.On("Get", identityKey).Once().Return(id, nil)

	SetIdentity(c, id)
	res, err := GetIdentity(c)

	assert.Equal(t, id, res)
	assert.NoError(t, err)
}

func TestIdentityFail(t *testing.T) {
	c := &contextMock{}
	c.On("Get", identityKey).Once().Return(nil, errors.New("err"))

	res, err := GetIdentity(c)

	assert.Nil(t, res)
	assert.Error(t, err)
}
