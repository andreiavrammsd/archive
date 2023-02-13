SOURCEDIR = src
HEADERDIR = include/Apixu
TESTDIR = tests
EXAMPLEDIR = examples

SOURCES = $(shell find $(SOURCEDIR) -name "*.cpp")
HEADERS = $(shell find $(HEADERDIR) -name "*.h")
TESTS = $(shell find $(TESTDIR) -name "*.cpp")
EXAMPLES = $(shell find $(EXAMPLEDIR) -name "*.cpp")

all: build

env:
	sudo apt install -y gcc make build-essential valgrind linux-tools-$(shell uname -r)

dep:
	sudo apt install -y libcurl4-openssl-dev lcov gcovr

lint:
	@python dev/cpplint.py --linelength=120 \
		--filter=-runtime/indentation_namespace,-whitespace/indent,-whitespace/braces,-whitespace/newline \
		--quiet \
		$(HEADERS) $(SOURCES) $(TESTS) $(EXAMPLES)

build:
	docker build -f dev/Dockerfile-build -t apixubuild .
	docker run --rm -ti -v $(PWD):/src -w /src apixubuild

api-up:
	docker build -f dev/Dockerfile-api -t apixuapi dev
	docker run -tid --rm -p 5000:5000 --name=apixuapi apixuapi

api-down:
	docker stop apixuapi || true
	docker rmi  apixuapi || true
