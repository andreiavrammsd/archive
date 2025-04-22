.PHONY: build clean qa

FILES=$(shell find . -regextype egrep -regex '.*.+\.(cpp|h)' -not -path './cmake-build-*/*')

build:
	mkdir -p build && cd build && cmake .. && make -j

clean:
	rm -rf build

qa:
	@[ $(shell which cppcheck) ] || sudo apt install -y cppcheck
	@cppcheck --enable=warning,performance,portability,style --language=c++ --quiet $(FILES)
