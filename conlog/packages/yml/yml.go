// Package yml provides a function to directly read
// an yml file into provided structure
package yml

import (
	"os"

	"gopkg.in/yaml.v2"
)

var readFile = os.ReadFile
var ymlUnmarshal = yaml.Unmarshal

func Read(file string, out interface{}) error {
	content, err := readFile(file)
	if err != nil {
		return err
	}

	err = ymlUnmarshal(content, out)
	if err != nil {
		return err
	}

	return nil
}
