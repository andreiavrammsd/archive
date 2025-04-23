#!/usr/bin/env sh
set -e

sed -i "s/<app>/$APP/g" /.fsw.yml

cd /conlog/apps/$APP

exec "$@"
