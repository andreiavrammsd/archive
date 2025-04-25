package storage

import (
	"context"
)

type Writer interface {
	Write(ctx context.Context, obj interface{}) (err error)
}

type BatchWriter interface {
	Write(ctx context.Context, batch []interface{}) (err error)
}
