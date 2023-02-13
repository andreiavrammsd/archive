#!/usr/bin/env sh

make clean && make && make install && php test.php
