#!/usr/bin/env bash

NAME=$1
PATH_TO_BINARY=$2
EXPORT=$3
INIT_FILE=$4

sed -i -E "s#(Provides:)#\1 $NAME#" $INIT_FILE
sed -i -E "s#(Author:)#\1 $NAME#" $INIT_FILE
sed -i -E "s#(DESC=)#\1\"$NAME\"#" $INIT_FILE
sed -i -E "s#(DAEMON=)(.*)#\1$PATH_TO_BINARY#" $INIT_FILE
sed -i -E "s#LOGFILENAME#$NAME#" $INIT_FILE

IFS=';' read -r -a array <<< "$EXPORT"
for var in "${array[@]}"
do
    echo "export $var" >> $INIT_FILE
done
