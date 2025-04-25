package tasks

import (
	"context"
	"encoding/json"
	"fmt"
	"net/http"
	"sync"
	"time"

	"go.mongodb.org/mongo-driver/mongo"
)

type HealthService interface {
	Ping() error
	ID() string
}

type MongoHealth struct {
	Client *mongo.Client
}

func (m *MongoHealth) Ping() error {
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*5)
	defer cancel()
	return m.Client.Ping(ctx, nil)
}

func (m *MongoHealth) ID() string {
	return "mongo"
}

type Health struct {
	Services []HealthService
	Period   time.Duration
	status   map[string]error
	lock     sync.RWMutex
}

func (h *Health) Concurrency() uint {
	return 1
}

func (h *Health) Ping() (err error) {
	if h.status == nil {
		h.status = make(map[string]error)
	}

	h.lock.Lock()
	defer h.lock.Unlock()

	for i := 0; i < len(h.Services); i++ {
		err = h.Services[i].Ping()
		if err != nil {
			err = fmt.Errorf("health check error at %s: %s", h.Services[i].ID(), err)
			h.status[h.Services[i].ID()] = err
		} else {
			delete(h.status, h.Services[i].ID())
		}
	}

	return err
}

func (h *Health) Run() error {
	http.HandleFunc("/health", func(w http.ResponseWriter, r *http.Request) {
		h.lock.RLock()
		defer h.lock.RUnlock()

		var res interface{}
		if len(h.status) > 0 {
			w.WriteHeader(http.StatusInternalServerError)
			res = h.status
		} else {
			res = struct {
				Status string `json:"status"`
			}{
				Status: "ok",
			}
		}

		data, _ := json.Marshal(res)
		_, _ = w.Write(data)
	})

	go http.ListenAndServe(":8877", nil)

	for {
		h.Ping()
		time.Sleep(h.Period)
	}
}
