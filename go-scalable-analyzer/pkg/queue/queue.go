package queue

// EncoderDecoder must be implemented to pass and receive queue messages
type EncoderDecoder interface {
	Encoder
	Decoder
}

// EncoderDecoder must be implemented to pass queue messages
type Encoder interface {
	Encode() ([]byte, error)
}

// EncoderDecoder must be implemented to receive queue messages
type Decoder interface {
	Decode([]byte) error
}

// Writer must be implemented to handle queue message writes
type Writer interface {
	Write(EncoderDecoder) (err error)
}

// Reader must be implemented to handle queue message reads
type Reader interface {
	Read(Delivery) (err error)
}

// type BatchReader interface {
//	Read(Delivery) (err error)
//}

// Delivery function is called on message read from queue
type Delivery func([]byte) error
