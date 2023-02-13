FROM php:7.3-cli-alpine3.9

RUN apk add --no-cache --update \
    autoconf \
    alpine-sdk \
    freetype \
    libpng \
    libjpeg-turbo \
    freetype-dev \
    libpng-dev \
    jpeg-dev \
    libjpeg \
    libjpeg-turbo-dev \
    libstdc++ && \
    docker-php-ext-configure gd \
        --with-freetype-dir=/usr/lib/ \
        --with-png-dir=/usr/lib/ \
        --with-jpeg-dir=/usr/lib/ \
        --with-gd && \
    NUMPROC=$(grep -c ^processor /proc/cpuinfo 2>/dev/null || 1) && \
    docker-php-ext-install -j${NUMPROC} gd && \
    printf "\n" | pecl install swoole-4.2.13 && \
    echo "extension=swoole.so" > /usr/local/etc/php/conf.d/php-ext-swoole.ini && \
    apk del --purge autoconf alpine-sdk && \
    rm -r /tmp/pear/
