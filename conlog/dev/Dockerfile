FROM golang:1.24.2-alpine

RUN apk add --update --no-cache \
    wget git mercurial openssh-client && \
    mkdir -p $GOPATH/bin && \
    wget https://github.com/google/protobuf/releases/download/v3.5.1/protoc-3.5.1-linux-x86_64.zip && \
    mkdir protoc && \
    unzip protoc-3.5.1-linux-x86_64.zip -d protoc && \
    mv protoc/bin /usr/local && \
    mv protoc/include /usr/local && \
    rm -r protoc && rm protoc-3.5.1-linux-x86_64.zip && \
    go install github.com/codeskyblue/fswatch@892e1e55b6025d5ef90bbc37f6e208da568d9fa0

RUN apk add uuidgen && uuidgen | tr -d '-' > /etc/machine-id

COPY ./entrypoint.sh /
RUN chmod +x /entrypoint.sh

COPY ./.fsw.yml /.fsw.yml

ENTRYPOINT ["/entrypoint.sh"]

CMD fswatch -config /.fsw.yml 
