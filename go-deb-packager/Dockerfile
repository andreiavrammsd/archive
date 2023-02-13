FROM ubuntu:16.10

RUN apt-get update && apt-get install wget nano git make -y

RUN wget https://storage.googleapis.com/golang/go1.7.linux-amd64.tar.gz \
    && tar -xvf go1.7.linux-amd64.tar.gz \
    && mv go /usr/local \
    && rm go1.7.linux-amd64.tar.gz \
    && rm -rf /var/lib/apt/lists/ \
    && apt-get clean && apt-get autoremove && apt-get autoclean
    
COPY entrypoint.sh /entrypoint.sh
RUN /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

WORKDIR /pkg
