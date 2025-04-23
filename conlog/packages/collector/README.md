# ConLog

## Collector

### Development ###
* Install: https://github.com/google/protobuf/releases (bin + types)
* Generate protobuf (from packages directory): sudo protoc -I collector/ collector/collector.proto --go_out=plugins=grpc:collector
