FROM ubuntu:16.04

RUN apt-get update -y
RUN apt-get install -y nano wget curl unzip
RUN apt-get install -y php7.0-cli php7.0-common php7.0-dev
RUN apt-get install -y libevent-dev

COPY php.ini /etc/php/7.0/cli/conf.d/php.ini

WORKDIR /tmp
RUN wget https://github.com/expressif/pecl-event-libevent/archive/master.zip
RUN unzip master.zip
WORKDIR /tmp/pecl-event-libevent-master
RUN phpize && ./configure && make && make install
RUN echo 'extension=libevent.so' > /etc/php/7.0/mods-available/libevent.ini
RUN ln -s /etc/php/7.0/mods-available/libevent.ini /etc/php/7.0/cli/conf.d/20-libevent.ini

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

RUN apt-get install composer -y

RUN apt-get install supervisor -y
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

COPY install.sh /install.sh
RUN chmod +x /install.sh

WORKDIR /app

CMD ["/bin/bash", "/install.sh"]
