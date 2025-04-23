// Package e holds error handling functions
package e

import "log"

func LogFatalIfError(err error) {
	if err != nil {
		log.Fatal(err)
	}
}
