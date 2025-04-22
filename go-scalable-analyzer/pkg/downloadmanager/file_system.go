package downloadmanager

import (
	"io"
	"os"
)

type FileSystemConfig struct {
}

type fileSystemDownloadManager struct {
}

func (fs *fileSystemDownloadManager) DownloadToFile(path string, w io.Writer) (err error) {
	f, err := os.Open(path)
	if err != nil {
		return
	}

	defer func() {
		if e := f.Close(); e != nil {
			err = e
		}
	}()

	if _, err := io.Copy(w, f); err != nil {
		return err
	}

	return nil
}
