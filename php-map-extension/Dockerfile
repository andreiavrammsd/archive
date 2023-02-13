FROM php:7.2-cli-alpine3.7

RUN apk add --update --no-cache git make alpine-sdk && \
    git clone https://github.com/CopernicaMarketingSoftware/PHP-CPP && \
    cd PHP-CPP && \
    git checkout v2.1.0 && \
    mv Makefile Makefile.OLD && \
    sed 's|^PHP_LINKER_FLAGS.*|PHP_LINKER_FLAGS=${LINKER_FLAGS} -I/usr/src/php -I/usr/src/php/main -I/usr/src/php/Zend -I/usr/src/php/TSRM -I/usr/src/php/ext -I/usr/src/php/ext/date/lib|' Makefile.OLD > Makefile && \
    docker-php-source extract && \
    make -j$(nproc) && \
    make install

WORKDIR /src/map

# https://github.com/CopernicaMarketingSoftware/PHP-CPP/issues/355#issuecomment-378432895
