package netproc

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestGetState(t *testing.T) {
	for k, v := range states {
		assert.Equal(t, v, GetState(k))
	}
}
