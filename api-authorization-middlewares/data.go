package main

var Users = []User{
	{
		ID:          1,
		Username:    "admin",
		Password:    "admin",
		Type:        AdminAccount,
		Permissions: CanSeeUsers | CanCreateUsers | CanDeleteUsers,
	},
	{
		ID:          2,
		Username:    "moderator",
		Password:    "moderator",
		Type:        ModeratorAccount,
		Permissions: CanSeeUsers | CanDeleteUsers,
	},
	{
		ID:          3,
		Username:    "moderator2",
		Password:    "moderator2",
		Type:        ModeratorAccount,
		Permissions: CanManageUsers,
	},
	{
		ID:          4,
		Username:    "user",
		Password:    "user",
		Type:        UserAccount,
		Permissions: 0,
	},
}

type User struct {
	ID          int         `json:"id"`
	Username    string      `json:"username"`
	Password    string      `json:"-"`
	Type        AccountType `json:"type"`
	Permissions Permission  `json:"-"`
}

type AccountType int

const (
	AdminAccount AccountType = iota
	ModeratorAccount
	UserAccount
)
