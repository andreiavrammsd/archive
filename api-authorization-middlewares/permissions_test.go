package main

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

var aclTestData = []struct {
	user      Permission
	handler   []Permission
	assertion assert.BoolAssertionFunc
}{
	{
		CanManageUsers | CanCreateUsers,
		[]Permission{CanManageUsers},
		assert.True,
	},
	{
		CanSeeUsers,
		[]Permission{CanManageUsers},
		assert.False,
	},
	{
		CanSeeUsers | CanDeleteUsers,
		[]Permission{0},
		assert.False,
	},
	{
		0,
		[]Permission{CanManageUsers, CanCreateUsers},
		assert.False,
	},
}

func TestACL_Can(t *testing.T) {
	for i, data := range aclTestData {
		acl := ACL{data.user}

		t.Run(fmt.Sprintf("can #%d", i+1), func(t *testing.T) {
			data.assertion(t, acl.Can(data.handler...))
		})

	}
}
