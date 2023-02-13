# Go QA tools

Integrate tests and quality tools (linter, vet, static analysis, security and others) into your continuous integration/delivery system.
Based on [Go Meta Linter](https://github.com/alecthomas/gometalinter).

## Requirements
* Docker

## Usage
* Adjust [Makefile](Makefile) to your needs, build and push to your Docker repository.
* Pull the Docker image on the CI machine
* Run tools:
```
docker run -ti --rm \
    -e PKG=github.com/andreiavrammsd/dotenv-editor \
    -e CONFIG=dev/.gometalinter.json \
    -v $PWD:/app \
    yourdockerusername/go-qa-tools \
    make
```

Variables:
* PKG: Full package name (github.com/andreiavrammsd/dotenv-editor).
* CONFIG: Path to your custom gometalinter json file file. If empty, the [default config](.gometalinter.json) is used.

Volume:
* Your source code directory must be mounted as a volume to /app.

## Example
Basic example for [Travis CI](https://travis-ci.org/)

```
sudo: required

language: minimal

install:
- docker pull andreiavrammsd/go-qa-tools

script:
- docker run -ti -e PKG=github.com/andreiavrammsd/dotenv-editor -e CONFIG=dev/.gometalinter.json -v $PWD:/app andreiavrammsd/go-qa-tools make
```
