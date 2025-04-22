package storage

// Writer must be implemented to write data to storage.
type Writer interface {
	// Write writes data and returns inserted id and error
	Write(obj interface{}) (interface{}, error)
	Update(id, obj interface{}) error
}

// BatchWriter must be implemented to write batched data to storage.
type BatchWriter interface {
	Write(batch []interface{}) (err error)
}
