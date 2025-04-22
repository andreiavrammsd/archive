package downloadmanager

import (
	"fmt"
	"io"
	"net/http"
)

const s3BaseURL = "http://ipv4.download.thinkbroadband.com/"

type ThinkBroadbandConfig struct {
	Username string `json:"username"`
}

type thinkBroadbandDownloadManager struct {
	config     ThinkBroadbandConfig
	httpClient *http.Client
}

func (tbb *thinkBroadbandDownloadManager) DownloadToFile(id string, w io.Writer) (err error) {
	res, err := tbb.httpClient.Get(s3BaseURL + id + "?token=" + tbb.config.Username)
	if err != nil {
		return err
	}

	if res.StatusCode != http.StatusOK {
		return fmt.Errorf("cannot download file at url: %s (response status: %d)", s3BaseURL+id, res.StatusCode)
	}

	defer func() {
		if e := res.Body.Close(); e != nil {
			err = e
		}
	}()

	if _, err := io.Copy(w, res.Body); err != nil {
		return err
	}

	return nil
}
