FROM ubuntu:16.04

RUN apt-get update -y
RUN apt-get install -y nano wget curl unzip
RUN apt-get install -y php7.0-cli php7.0-common php7.0-dev
RUN apt-get install -y libevent-dev

WORKDIR /tmp
RUN wget https://github.com/expressif/pecl-event-libevent/archive/master.zip
RUN unzip master.zip
WORKDIR /tmp/pecl-event-libevent-master
RUN phpize && ./configure && make && make install
RUN echo 'extension=libevent.so' > /etc/php/7.0/mods-available/libevent.ini
RUN ln -s /etc/php/7.0/mods-available/libevent.ini /etc/php/7.0/cli/conf.d/20-libevent.ini

RUN apt-get install composer -y

RUN curl -s https://packagecloud.io/install/repositories/phalcon/stable/script.deb.sh | bash
RUN apt-get install php7.0-phalcon

RUN apt-get install supervisor -y

COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

COPY install.sh /install.sh
RUN chmod +x /install.sh

WORKDIR /zonga

CMD ["/bin/bash", "/install.sh"]
