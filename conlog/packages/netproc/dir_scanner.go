package netproc

import (
	"os"
	"path/filepath"
	"sync"
)

const PROC_FILE_PATTERN = "/proc/[0-9]*/fd/[0-9]*"

type processDirScanner struct {
}

var osReadlink = os.Readlink

func (pds *processDirScanner) Scan() (map[string]string, error) {
	paths, err := pds.getPaths()
	if err != nil {
		return make(map[string]string), err
	}

	wg := sync.WaitGroup{}
	var dirMap sync.Map

	for _, item := range paths {
		wg.Add(1)

		go func(item string) {
			path, err := osReadlink(item)
			if err == nil {
				dirMap.Store(item, path)
			}
			wg.Done()
		}(item)
	}

	wg.Wait()

	return pds.getDirsArray(&dirMap), nil
}

var filepathGlob = filepath.Glob

func (pds *processDirScanner) getPaths() ([]string, error) {
	paths, err := filepathGlob(PROC_FILE_PATTERN)
	if err != nil {
		return nil, err
	}

	return paths, nil
}

func (pds *processDirScanner) getDirsArray(dirMap *sync.Map) map[string]string {
	dirs := make(map[string]string)
	dirMap.Range(func(key, value interface{}) bool {
		dirs[key.(string)] = value.(string)
		return true
	})
	return dirs
}

func NewProcessDirScanner() ProcessDirScanner {
	return &processDirScanner{}
}
