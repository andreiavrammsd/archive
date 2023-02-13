FROM ubuntu:16.04

RUN apt-get update -y
RUN apt-get install -y nano wget curl unzip
RUN apt-get install -y php7.0-cli php7.0-common php7.0-dev

WORKDIR /tmp
RUN apt-get install -y libtool pkg-config build-essential autoconf automake
RUN wget https://github.com/zeromq/zeromq4-1/releases/download/v4.1.5/zeromq-4.1.5.tar.gz
RUN tar -xzf zeromq-4.1.5.tar.gz
WORKDIR /tmp/zeromq-4.1.5
RUN ./configure
RUN make && make install

RUN yes 'autodetect' | pecl install zmq-1.1.3
RUN echo "extension=zmq.so" >> /etc/php/7.0/mods-available/zmq.ini
RUN phpenmod zmq

WORKDIR /app
