package main

import (
	"errors"

	"github.com/labstack/echo"
)

const identityKey = "identity"

type Identity struct {
	Username    string
	Type        AccountType
	Permissions Permission
}

func SetIdentity(c echo.Context, identity *Identity) {
	c.Set(identityKey, identity)
}

func GetIdentity(c echo.Context) (identity *Identity, err error) {
	var ok bool
	if identity, ok = c.Get(identityKey).(*Identity); !ok {
		err = errors.New("identity not set")
	}
	return
}
