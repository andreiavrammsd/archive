#!/bin/bash

composer install --no-ansi --no-interaction --no-progress --optimize-autoloader
/usr/bin/supervisord
