package downloadmanager

import (
	"errors"
	"io"
	"net"
	"net/http"
	"time"
)

const (
	dialTimeout     = time.Second * 5
	idleConnTimeout = time.Second * 60
	maxIdleConn     = 100
	requestTimeout  = time.Minute * 200
)

type DownloadManager interface {
	DownloadToFile(id string, file io.Writer) error
}

type Provider string

const (
	ThinkBroadband Provider = "thinkbroadbrand"
	FileSystem     Provider = "fs"
)

func New(config interface{}) (m DownloadManager, err error) {
	switch c := config.(type) {
	case *ThinkBroadbandConfig:
		m = &thinkBroadbandDownloadManager{
			*c,
			getHTTPClient(),
		}
	case *FileSystemConfig:
		m = &fileSystemDownloadManager{}
	default:
		err = errors.New("download manager not found")
	}

	return
}

func getHTTPClient() *http.Client {
	netTransport := &http.Transport{
		DialContext: (&net.Dialer{
			Timeout: dialTimeout,
		}).DialContext,
		TLSHandshakeTimeout: dialTimeout,
		MaxIdleConns:        maxIdleConn,
		IdleConnTimeout:     idleConnTimeout,
	}

	return &http.Client{
		Timeout:   requestTimeout,
		Transport: netTransport,
	}
}
