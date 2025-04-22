BUILD_DIR = build
CMAKE = cmake
CTEST = ctest

.PHONY: all build test clean

all: build test

build:
	@${CMAKE} -B ${BUILD_DIR}
	@make -C ${BUILD_DIR}

test: build
	@${CTEST} --test-dir ${BUILD_DIR} --output-on-failure

clean:
	@rm -rf ${BUILD_DIR}
