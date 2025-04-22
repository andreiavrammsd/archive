package health

import (
	"context"
	"time"

	"go.mongodb.org/mongo-driver/mongo"
)

type Mongo struct {
	Client *mongo.Client
}

func (m *Mongo) Ping() error {
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*5)
	defer cancel()
	return m.Client.Ping(ctx, nil)
}

func (m *Mongo) ID() string {
	return "mongo"
}
