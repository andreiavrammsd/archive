// Package netproc provides os interaction to retrieve network processes
// and associated data (pid, executable name, username, state, ip, port etc)
package netproc

// Uses Netstat to request processes by protocol
type ProcessReader interface {
	Read(protocols []string) (Processes, error)
}

// Main processes info retrieval
type Netstat interface {
	GetProcesses(proto string) (Processes, error)
}

// Process details based on inode
type ProcessIdentification interface {
	Identify(p Process, dirs map[string]string) Process
}

// Process directories scanner
type ProcessDirScanner interface {
	Scan() (map[string]string, error)
}
