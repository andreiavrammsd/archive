package main

import (
	"time"

	"github.com/dgrijalva/jwt-go"
)

const (
	JWTSecret   = `8o\!DO#=il+9."07t%f\qC\!ux_nTVJ)d,Dh/92?b*Wd}vZu)p$6lauP3\C~}j$`
	tokenExpire = time.Hour * 72
)

func Authorize(u User) (string, error) {
	token := jwt.New(jwt.SigningMethodHS256)

	claims := token.Claims.(jwt.MapClaims)
	claims["username"] = u.Username
	claims["permissions"] = u.Permissions
	claims["type"] = u.Type
	claims["exp"] = time.Now().Add(tokenExpire).Unix()

	return token.SignedString([]byte(JWTSecret))
}
