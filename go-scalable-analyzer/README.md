# Scalable Analyzer

## Development

### Requirements
* Make
* Git
* [Go 1.13+](https://golang.org/dl/)
* [Docker](https://docs.docker.com/install/)
* [Docker Compose](https://docs.docker.com/compose/install/)

### Start environment
```bash
make up
make run APP=api
make run APP=downloadmanager
make run APP=analyzer
```

Send analysis requests: `make run APP=client`
