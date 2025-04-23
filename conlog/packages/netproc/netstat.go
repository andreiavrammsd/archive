// Based on GOnetstat https://github.com/drael/GOnetstat/
package netproc

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type netstat struct{}

func (n *netstat) GetProcesses(proto string) (Processes, error) {
	processes := make(Processes)

	data, err := getData(proto)
	if err != nil {
		return Processes{}, err
	}

	for _, line := range data {
		input := removeEmptyLines(strings.Split(strings.TrimSpace(line), " "))

		p := Process{}
		p.Protocol = proto
		p.UserID = input[7]
		p.PID = -1
		p.State = input[3]
		p.Inode = getInode(input[9])
		p.IP, p.Port = getIPAndport(input[1])
		p.ForeignIP, p.ForeignPort = getIPAndport(input[2])

		hash := getProcessHash(p)

		// Add time after generating hash
		p.Time = time.Now()

		processes[hash] = p
	}

	return processes, nil
}

var strconvAtoi = strconv.Atoi

func getInode(in string) int {
	inode, err := strconvAtoi(in)
	if err != nil {
		inode = -1
	}
	return inode
}

func getIPAndport(in string) (string, int64) {
	array := strings.Split(in, ":")
	ip := convertIP(array[0])
	port := hexToDec(array[1])
	return ip, port
}

var readFile = os.ReadFile

// getData reads network data from os
func getData(proto string) ([]string, error) {
	proc := fmt.Sprintf("/proc/net/%s", proto)

	data, err := readFile(proc)
	if err != nil {
		return []string{}, err
	}

	lines := strings.Split(string(data), "\n")

	// Remove header and blank line at the end
	return lines[1 : len(lines)-1], nil
}

func hexToDec(h string) int64 {
	d, err := strconv.ParseInt(h, 16, 32)
	if err != nil {
		log.Fatal(err)
	}
	return d
}

func convertIP(ip string) string {
	// Convert the ipv4 to decimal. Have to rearrange the ip because the
	// default value is in little Endian order.

	var out string

	// Check ip size if greater than 8 is a ipv6 type
	if len(ip) > 8 {
		i := []string{ip[30:32],
			ip[28:30],
			ip[26:28],
			ip[24:26],
			ip[22:24],
			ip[20:22],
			ip[18:20],
			ip[16:18],
			ip[14:16],
			ip[12:14],
			ip[10:12],
			ip[8:10],
			ip[6:8],
			ip[4:6],
			ip[2:4],
			ip[0:2]}
		out = fmt.Sprintf("%v%v:%v%v:%v%v:%v%v:%v%v:%v%v:%v%v:%v%v",
			i[14], i[15], i[13], i[12],
			i[10], i[11], i[8], i[9],
			i[6], i[7], i[4], i[5],
			i[2], i[3], i[0], i[1])

	} else {
		i := []int64{hexToDec(ip[6:8]),
			hexToDec(ip[4:6]),
			hexToDec(ip[2:4]),
			hexToDec(ip[0:2])}

		out = fmt.Sprintf("%v.%v.%v.%v", i[0], i[1], i[2], i[3])
	}
	return out
}

func getProcessHash(p Process) string {
	str := fmt.Sprintf("%v\n", p)

	hasher := md5.New()
	hasher.Write([]byte(str))
	hash := hex.EncodeToString(hasher.Sum(nil))

	return hash
}

func removeEmptyLines(array []string) []string {
	var result []string
	for _, i := range array {
		if i != "" {
			result = append(result, i)
		}
	}
	return result
}
