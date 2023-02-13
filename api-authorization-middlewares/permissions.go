package main

const (
	CanSeeUsers Permission = 1 << iota
	CanCreateUsers
	CanDeleteUsers
	CanManageUsers
)

type Permission uint8

type ACL struct {
	Permission
}

func (a ACL) Can(ps ...Permission) bool {
	for _, p := range ps {
		if a.Permission&p != 0 {
			return true
		}
	}

	return false
}
