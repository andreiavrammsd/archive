package logger

import (
	"io"
	"log"
	"os"
)

// Thread safe?
type Logger interface {
	Error(...interface{})
	Errorf(format string, args ...interface{})
	Fatal(...interface{})
	Fatalf(format string, args ...interface{})
	Info(...interface{})
	Infof(format string, args ...interface{})
}

type logger struct {
	out io.Writer
	err io.Writer
}

func (l *logger) Error(err ...interface{}) {
	log.Println(err...)
}

func (l *logger) Errorf(format string, args ...interface{}) {
	log.Printf(format, args...)
}

func (l *logger) Fatal(err ...interface{}) {
	log.Println(err...)
	os.Exit(1)
}

func (l *logger) Fatalf(format string, args ...interface{}) {
	log.Printf(format, args...)
	os.Exit(1)
}

func (l *logger) Info(msg ...interface{}) {
	log.Println(msg...)
}

func (l *logger) Infof(format string, args ...interface{}) {
	log.Printf(format, args...)
}

func New(out, err io.Writer) Logger {
	return &logger{
		out: out,
		err: err,
	}
}
