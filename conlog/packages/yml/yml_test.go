package yml

import (
	"errors"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestRead(t *testing.T) {
	readFile = func(filename string) ([]byte, error) {
		return []byte(""), nil
	}

	type YmlStruct struct {
		Top struct {
			Key   string   `yaml:"key"`
			Array []string `yaml:"array"`
		} `yaml:"client_id"`
	}

	ymlStruct := YmlStruct{}
	ymlUnmarshal = func(in []byte, out interface{}) (err error) {
		ymlStruct.Top.Key = "Value"
		ymlStruct.Top.Array = []string{"a", "b"}
		return nil
	}

	err := Read("file", ymlStruct)

	assert.Equal(t, "Value", ymlStruct.Top.Key)
	assert.Equal(t, []string{"a", "b"}, ymlStruct.Top.Array)
	assert.Nil(t, err)
}

func TestReadWhenFileReadFails(t *testing.T) {
	readFile = func(filename string) ([]byte, error) {
		ymlContent := ``
		return []byte(ymlContent), errors.New("")
	}

	err := Read("file", "")
	assert.NotNil(t, err)
}
