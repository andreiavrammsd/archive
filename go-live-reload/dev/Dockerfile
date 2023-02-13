FROM golang:1.10-alpine3.7

RUN apk add --update --no-cache git && \
    mkdir -p $GOPATH/bin && \
    go get github.com/codeskyblue/fswatch

ARG APP

WORKDIR /go/src/github.com/andreiavrammsd/go-live-reload/apps/${APP}

COPY ./.fsw.yml /.fsw.yml
RUN sed -i "s/<app>/${APP}/g" /.fsw.yml

CMD fswatch -config /.fsw.yml 
