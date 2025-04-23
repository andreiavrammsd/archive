// Based on GOnetstat https://github.com/drael/GOnetstat/
package netproc

import (
	"fmt"
	"os"
	"os/user"
	"regexp"
	"strconv"
	"strings"
)

// clear sometimes, maybe when on process delete | or maybe remove the vars
var exes map[int]string
var users map[string]string
var pids map[int]int

type processIdentification struct {
}

func (pi *processIdentification) Identify(p Process, dirs map[string]string) Process {
	pid := findPid(p.Inode, dirs)
	exe := getProcessExe(pid)

	p.PID = pid
	p.Exe = exe
	p.Name = getProcessName(exe)
	p.User = getUser(p.UserID)

	return p
}

func findPid(inode int, dirs map[string]string) int {
	// Loop through all fd dirs of process on /proc to compare the inode and
	// get the pid.

	if _, ok := pids[inode]; !ok {
		pids[inode] = -1

		re := regexp.MustCompile(strconv.Itoa(inode))
		for item, path := range dirs {
			out := re.FindString(path)
			if len(out) != 0 {
				pid, err := strconv.Atoi(strings.Split(item, "/")[2])
				if err == nil {
					pids[inode] = pid
				}
			}
		}
	}

	return pids[inode]
}

var osReadLink = os.Readlink

func getProcessExe(pid int) string {
	if _, ok := exes[pid]; !ok {
		exe := fmt.Sprintf("/proc/%d/exe", pid)
		path, _ := osReadLink(exe)
		exes[pid] = path
	}

	return exes[pid]
}

func getProcessName(exe string) string {
	n := strings.Split(exe, "/")
	name := n[len(n)-1]
	return strings.Title(name)
}

var userLookupId = user.LookupId

func getUser(uid string) string {
	if _, ok := users[uid]; !ok {
		u, err := userLookupId(uid)
		users[uid] = ""
		if err == nil {
			users[uid] = u.Username
		}
	}
	return users[uid]
}

func NewProcessIdentification() ProcessIdentification {
	exes = make(map[int]string)
	users = make(map[string]string)
	pids = make(map[int]int)

	return &processIdentification{}
}
