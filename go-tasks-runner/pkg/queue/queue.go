package queue

type EncoderDecoder interface {
	Encoder
	Decoder
}

type Encoder interface {
	Encode() ([]byte, error)
}

type Decoder interface {
	Decode([]byte) error
}

type Writer interface {
	Write(EncoderDecoder) (err error)
}

type Reader interface {
	Read(Delivery) (err error)
}

type Delivery func([]byte) error
