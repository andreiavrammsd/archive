package tasks

import (
	"encoding/json"
	"fmt"
	"io"
	"math"
	"os"
	"runtime"

	"github.com/andreiavrammsd/tasks-runner/pkg/analyzer"
	"github.com/andreiavrammsd/tasks-runner/pkg/downloadmanager"
	"github.com/andreiavrammsd/tasks-runner/pkg/queue"
)

type ClientMessage struct {
	Provider downloadmanager.Provider
	Username string
	File     string
	Unix     int64
}

func (c ClientMessage) Encode() ([]byte, error) {
	return json.Marshal(c)
}

func (c *ClientMessage) Decode(data []byte) error {
	return json.Unmarshal(data, c)
}

type DownloadFile struct {
	Reader queue.Reader
	Writer queue.Writer
}

func (d *DownloadFile) Concurrency() uint {
	return uint(math.Max(1, float64(runtime.NumCPU())-2))
}

func (d *DownloadFile) Run() error {
	return d.Reader.Read(d.download)
}

func (d *DownloadFile) download(data []byte) error {
	message := ClientMessage{}
	if err := message.Decode(data); err != nil {
		return err
	}

	dm, err := getDownloadManager(message, data)
	if err != nil {
		return err
	}

	filePath, err := Download(dm, message.File)
	if err != nil {
		return err
	}

	task := analyzer.Task{
		LocalFile: filePath,
		FileID:    message.File,
		MaxIssues: int(message.Unix),
	}

	return d.Writer.Write(&task)
}

type ProgressWriter struct {
	file     io.Writer
	fileName string
	total    uint64
}

func (pw *ProgressWriter) Write(p []byte) (n int, err error) {
	n, err = pw.file.Write(p)
	pw.total += uint64(n)
	return
}

func Download(dm downloadmanager.DownloadManager, from string) (string, error) {
	file, err := os.CreateTemp("/tmp", "dwn")
	if err != nil {
		return "", err
	}
	defer func() {
		if e := file.Close(); e != nil {
			err = e
		}
	}()

	pw := &ProgressWriter{
		file:     file,
		fileName: file.Name(),
	}
	if err := dm.DownloadToFile(from, pw); err != nil {
		return "", err
	}

	return file.Name(), nil
}

func getDownloadManager(m ClientMessage, data []byte) (manager downloadmanager.DownloadManager, err error) {
	var config interface{}

	switch m.Provider {
	case downloadmanager.ThinkBroadband:
		config = &downloadmanager.ThinkBroadbandConfig{}
	case downloadmanager.FileSystem:
		config = &downloadmanager.FileSystemConfig{}
	default:
		err = fmt.Errorf("unknown provider: %s", m.Provider)
		return
	}

	if err = json.Unmarshal(data, config); err != nil {
		return
	}

	manager, err = downloadmanager.New(config)

	return
}
