package main

import (
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func main() {
	e := echo.New()
	e.Use(middleware.Logger())
	e.Use(middleware.Recover())
	e.Use(middleware.RequestID())

	e.POST("/login", login)

	ug := e.Group("/users", AuthorizationMiddleware, SetIdentityMiddleware)
	ug.GET("", usersList, ACLMiddleware(CanSeeUsers))
	ug.POST("", usersCreate, ACLMiddleware(CanCreateUsers, CanManageUsers))
	ug.DELETE("/:id", usersDelete, AdminAccessOnlyMiddleware, ACLMiddleware(CanDeleteUsers))

	e.Logger.Fatal(e.Start(":8118"))
}
