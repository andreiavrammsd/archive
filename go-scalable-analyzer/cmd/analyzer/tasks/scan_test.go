package tasks

import (
	"errors"
	"testing"

	"github.com/andreiavrammsd/scalableanalyzer/pkg/analyzer"
	"github.com/andreiavrammsd/scalableanalyzer/pkg/queue"

	"github.com/stretchr/testify/assert"
)

type loggerMock struct {
}

func (l loggerMock) Fatal(...interface{}) {
}

func (l loggerMock) Fatalf(format string, args ...interface{}) {
}

func (l loggerMock) Error(...interface{}) {

}

func (l loggerMock) Errorf(format string, args ...interface{}) {

}

func (l loggerMock) Info(...interface{}) {

}

func (l loggerMock) Infof(format string, args ...interface{}) {

}

type analyzerMock struct {
	issueToWrite analyzer.Issue
	writer       queue.Writer
}

func (s *analyzerMock) Analyze(input analyzer.Task) error {
	return s.writer.Write(&s.issueToWrite)
}

type readerMock struct {
	msg string
}

func (r *readerMock) Read(delivery queue.Delivery) (err error) {
	return delivery([]byte(r.msg))
}

type writerMock struct {
	result *analyzer.Issue
}

func (w *writerMock) Write(msg queue.EncoderDecoder) (err error) {
	var ok bool
	if w.result, ok = msg.(*analyzer.Issue); !ok {
		return errors.New("message is not analyzer issue")
	}
	return nil
}

func TestAnalyzeFile_Run(t *testing.T) {
	// Result expected from analyze action
	expectedIssue := analyzer.Issue{
		IssueTypeID: 1,
		MatchedText: "text...",
	}

	// Task request queue reader
	taskRequest := `{ 
	   "LocalFile":"",
	   "FileID":"",
	   "MaxIssues":0,
	}`
	readerMock := &readerMock{
		msg: taskRequest,
	}

	// Analyze result queue writer
	writerMock := &writerMock{}

	// Analyzer
	analyzerMock := &analyzerMock{
		expectedIssue,
		writerMock,
	}

	// Analyze file task
	analyzeFileTask := &AnalyzeFile{
		Reader:   readerMock,
		Writer:   writerMock,
		Analyzer: analyzerMock,
		Logger:   &loggerMock{},
	}
	err := analyzeFileTask.Run()

	assert.NoError(t, err)
	assert.Equal(t, &expectedIssue, writerMock.result)
}
