FROM golang:1-alpine

RUN apk add curl make alpine-sdk git
RUN curl -L https://git.io/vp6lP | sh

COPY .gometalinter.json .
COPY Makefile .
