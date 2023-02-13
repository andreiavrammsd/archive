package main

import (
	"net/http"

	"github.com/dgrijalva/jwt-go"
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

// AuthorizationMiddleware handles main authorization
var AuthorizationMiddleware = middleware.JWT([]byte(JWTSecret))

// SetIdentityMiddleware sets on context an Identity object which hold user information
// This makes easier retrieving user in further middleware functions
func SetIdentityMiddleware(next echo.HandlerFunc) echo.HandlerFunc {
	return func(c echo.Context) error {
		user := c.Get("user").(*jwt.Token)
		claims := user.Claims.(jwt.MapClaims)

		id := &Identity{
			Username:    claims["username"].(string),
			Permissions: Permission(claims["permissions"].(float64)),
			Type:        AccountType(claims["type"].(float64)),
		}

		SetIdentity(c, id)

		return next(c)
	}
}

// ACLMiddleware checks permissions for each handles against user's permissions
func ACLMiddleware(p ...Permission) echo.MiddlewareFunc {
	return func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			id, err := GetIdentity(c)
			if err != nil {
				c.Logger().Error(err)
				return c.NoContent(http.StatusForbidden)
			}

			acl := ACL{id.Permissions}
			if !acl.Can(p...) {
				return c.NoContent(http.StatusForbidden)
			}

			return next(c)
		}
	}
}

// AdminAccessOnlyMiddleware allows calls to pass only if user is of type AdminAccount
func AdminAccessOnlyMiddleware(next echo.HandlerFunc) echo.HandlerFunc {
	return func(c echo.Context) error {
		id, err := GetIdentity(c)
		if err != nil {
			c.Logger().Error(err)
			return c.NoContent(http.StatusForbidden)
		}

		if id.Type != AdminAccount {
			return c.NoContent(http.StatusForbidden)
		}

		return next(c)
	}
}
