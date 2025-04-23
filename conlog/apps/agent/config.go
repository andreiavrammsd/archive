package main

import (
	"errors"
	"fmt"
	"strings"
	"time"

	"github.com/andreiavrammsd/conlog/packages/yml"
	"gopkg.in/validator.v2"
)

type Protocols []string

type Config struct {
	Agent struct {
		ClientID     string        `yaml:"client_id" validate:"min=3,max=40,regexp=^[a-zA-Z0-9]*$"`
		ClientSecret string        `yaml:"client_secret" validate:"min=3,max=40,regexp=^[a-zA-Z]*$"`
		Protocols    Protocols     `yaml:"protocols" validate:"min=1,validateProtocols"`
		Delay        time.Duration `yaml:"delay" validate:"min=50"`
	} `yaml:"agent"`
}

var availableProtocols = Protocols{
	"tcp",
	"tcp6",
	"udp",
	"udp6",
	"raw",
	"raw6",
}

func validateProtocols(v interface{}, param string) error {
	protocols := v.(Protocols)

	for _, p := range protocols {
		found := false
		for _, ap := range availableProtocols {
			if p == ap {
				found = true
				break
			}
		}

		if !found {
			return errors.New(fmt.Sprintf(
				"Invalid protocol: %s. Available protocols are: %s.",
				p,
				strings.Join(availableProtocols, ", "),
			))
		}
	}

	return nil
}

func ValidateConfig(config Config) error {
	validator.SetValidationFunc("validateProtocols", validateProtocols)

	if errs := validator.Validate(config); errs != nil {
		return errs
	}

	return nil
}

var ymlRead = yml.Read

func NewConfig(file string) (Config, error) {
	config := Config{}

	err := ymlRead(file, &config)
	if err != nil {
		return config, err
	}

	return config, nil
}
