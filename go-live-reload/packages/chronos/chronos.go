package chronos

import "time"

func GetSeconds() int {
	return time.Now().Second()
}
