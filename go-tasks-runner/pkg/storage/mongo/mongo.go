package mongo

import (
	"context"

	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

type ClientConfig struct {
	Host     string
	Username string
	Password string
}

// add ping and reconnect
func NewClient(ctx context.Context, config ClientConfig) (*mongo.Client, error) {
	auth := options.Credential{
		Username: config.Username,
		Password: config.Password,
	}

	return mongo.Connect(ctx, options.Client().ApplyURI("mongodb://"+config.Host).SetAuth(auth))
}

type WriterConfig struct {
	Client     *mongo.Client
	Database   string
	Collection string
}

type Writer struct {
	collection *mongo.Collection
}

func (w *Writer) Write(ctx context.Context, obj interface{}) (err error) {
	_, err = w.collection.InsertOne(ctx, obj)
	return
}

type BatchWriter struct {
	collection *mongo.Collection
}

func (w *BatchWriter) Write(ctx context.Context, obj []interface{}) (err error) {
	_, err = w.collection.InsertMany(ctx, obj)
	return
}

func NewWriter(config *WriterConfig) *Writer {
	return &Writer{
		collection: config.Client.Database(config.Database).Collection(config.Collection),
	}
}

func NewBatchWriter(config *WriterConfig) *BatchWriter {
	return &BatchWriter{
		collection: config.Client.Database(config.Database).Collection(config.Collection),
	}
}
