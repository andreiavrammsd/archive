IMAGE := andreiavrammsd/go-qa-tools

ifndef CONFIG
CONFIG := /go/.gometalinter.json
endif

BASE := `dirname $(PKG)`

qa: check-params
	@echo "Running..."
	@mkdir -p src/$(BASE)
	@ln -s /app src/$(PKG)
	@cd src/$(PKG) && \
		go get -t ./... && \
		go test ./... && \
		gometalinter --config $(CONFIG) ./...

build:
	docker build . -t $(IMAGE)

push:
	docker push $(IMAGE)

clean:
	docker rmi -f $(IMAGE) || true

check-params:
	@if test "$(PKG)" = "" ; then \
		echo "PKG not set"; \
		exit 1; \
	fi
