package main

import (
	"net/http"
	"strconv"

	"github.com/labstack/echo"
)

func login(c echo.Context) error {
	login := &struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}{}
	if err := c.Bind(login); err != nil {
		return err
	}

	for _, u := range Users {
		if u.Username == login.Username && u.Password == login.Password {
			token, err := Authorize(u)
			if err != nil {
				return err
			}

			return c.JSON(http.StatusOK, map[string]string{
				"token": token,
			})
		}
	}

	return c.NoContent(http.StatusNotFound)
}

func usersList(c echo.Context) error {
	return c.JSON(http.StatusOK, Users)
}

func usersCreate(c echo.Context) (err error) {
	user := &User{}
	if err = c.Bind(user); err != nil {
		return err
	}

	for _, u := range Users {
		if u.Username == user.Username {
			return c.JSON(http.StatusBadRequest, map[string]string{
				"error": "User exists",
			})
		}
	}

	user.ID = len(Users) + 1
	user.Permissions = CanSeeUsers
	Users = append(Users, *user)

	return c.JSON(http.StatusCreated, user)
}

func usersDelete(c echo.Context) error {
	name := c.Param("id")
	id, err := strconv.Atoi(name)
	if err != nil {
		return c.NoContent(http.StatusBadRequest)
	}

	for i, u := range Users {
		if u.ID == id {
			Users = append(Users[:i], Users[i+1:]...)
			return c.NoContent(http.StatusOK)
		}
	}

	return c.NoContent(http.StatusNotFound)
}
