# php-map-extension

Simple map class built as PHP extension with [PHP-CPP](http://www.php-cpp.com/)

## Requires
[Docker](https://www.docker.com/)

## Build environment
docker build . -t php-map-extension

## Open container
docker run -ti --rm -v $PWD:/src php-map-extension sh

## Build extension and run test file (inside container)
[./build.sh](./map/build.sh)
