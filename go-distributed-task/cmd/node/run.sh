#!/bin/bash

pkill dtask

go build -race -o dtask

ports=( 8080 8081 8082 )

for port in "${ports[@]}"
do
    ./dtask -addr=":${port}" >> dtask.log 2>&1 &
    sleep 1
done


tail -f dtask.log
