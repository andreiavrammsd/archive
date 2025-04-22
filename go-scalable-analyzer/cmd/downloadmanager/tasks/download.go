package tasks

import (
	"encoding/json"
	"fmt"
	"io"
	"math"
	"os"
	"runtime"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/downloadmanager"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue"
)

type ClientMessage struct {
	Provider downloadmanager.Provider
	Username string
	File     string
	Unix     int64
}

func (c *ClientMessage) Encode() ([]byte, error) {
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

	//message := ClientMessage{}
	req := &analyzer.Request{}
	if err := req.Decode(data); err != nil {
		return fmt.Errorf("analyze request decode error: %s", err)
	}

	dm, err := getDownloadManager(req, data)
	if err != nil {
		return fmt.Errorf("download manager factory: %s", err)
	}

	filePath, err := Download(dm, req.File)
	if err != nil {
		return fmt.Errorf("error downloading file: %s (%s)", req.File, err)
	}

	task := analyzer.Task{
		LocalFile: filePath,
		FileID:    req.File,
		AnalyzeID: req.AnalyzeID,
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

	// cancellation required?

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

func getDownloadManager(m *analyzer.Request, data []byte) (manager downloadmanager.DownloadManager, err error) {
	// Config
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

	// Download manager
	manager, err = downloadmanager.New(config)
	return
}
