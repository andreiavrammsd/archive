.PHONY: all test lint coverage

COVER_PROFILE=cover.out
GOLINT := $(shell which golint)

all: test lint

up:
	docker-compose up -d

run:
	go run ./cmd/$(APP)

test:
	go test -race -cover -v ./...

lint:
ifndef GOLINT
		go get -u golang.org/x/lint/golint
endif
	golint $(go list ./...)

	@[ ! -f ./bin/golangci-lint ] && curl -sfL https://install.goreleaser.com/github.com/golangci/golangci-lint.sh \
		| sh -s -- -b ./bin v1.21.0 || true
	./bin/golangci-lint run

coverage:
	go test -race -v -coverprofile $(COVER_PROFILE) ./... && go tool cover -html=$(COVER_PROFILE)

prepushhook:
	echo '#!/bin/sh\n\nmake' > .git/hooks/pre-push && chmod +x .git/hooks/pre-push
