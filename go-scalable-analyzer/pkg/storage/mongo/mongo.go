package mongo

import (
	"context"
	"time"

	"go.mongodb.org/mongo-driver/bson"

	"go.mongodb.org/mongo-driver/bson/primitive"

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
	Timeout    time.Duration
}

type Writer struct {
	collection *mongo.Collection
	timeout    time.Duration
}

func (w *Writer) Write(obj interface{}) (id interface{}, err error) {
	ctx, cancel := context.WithTimeout(context.Background(), w.timeout)
	defer cancel()

	result, err := w.collection.InsertOne(ctx, obj)
	if err != nil {
		return
	}

	id = result.InsertedID.(primitive.ObjectID)

	return
}

func (w *Writer) Update(id, obj interface{}) (err error) {
	ctx, cancel := context.WithTimeout(context.Background(), w.timeout)
	defer cancel()

	mongoID, err := primitive.ObjectIDFromHex(id.(string))
	if err != nil {
		return
	}

	filter := bson.D{{"_id", mongoID}}
	update := bson.D{
		{"$set", obj},
	}

	_, err = w.collection.UpdateOne(ctx, filter, update)

	return
}

type BatchWriter struct {
	*Writer
}

func (w *BatchWriter) Write(obj []interface{}) (err error) {
	ctx, cancel := context.WithTimeout(context.Background(), w.timeout)
	defer cancel()

	_, err = w.collection.InsertMany(ctx, obj)
	return
}

func NewWriter(config *WriterConfig) *Writer {
	return &Writer{
		collection: config.Client.Database(config.Database).Collection(config.Collection),
		timeout:    config.Timeout,
	}
}

func NewBatchWriter(config *WriterConfig) *BatchWriter {
	return &BatchWriter{
		Writer: NewWriter(config),
	}
}
