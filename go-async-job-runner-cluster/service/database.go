package service

import (
	"fmt"
	"sync"
)

type Database interface {
	Store(key interface{}, value interface{})
	Delete(key interface{})
	GetAll() *sync.Map
	GetByKey(key interface{}) (interface{}, error)
}

type database struct {
	storage *sync.Map
}

func (db *database) Store(key interface{}, value interface{}) {
	db.storage.Store(key, value)
}

func (db *database) Delete(key interface{}) {
	db.storage.Delete(key)
}

func (db *database) GetAll() *sync.Map {
	return db.storage
}

func (db *database) GetByKey(key interface{}) (interface{}, error) {
	value, ok := db.storage.Load(key)

	if !ok {
		return value, fmt.Errorf("Object with key %v not found", key)
	}

	return value, nil
}

func NewDatabase() Database {
	var storage sync.Map
	db := &database{&storage}
	return db
}
