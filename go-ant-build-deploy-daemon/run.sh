#!/usr/bin/env bash

project=$1

if [[ -z "$project" ]]; then
    echo Usage: ./$(basename $0) projectname
else
    ant -propertyfile projects/$1.properties -lib lib/jsch-0.1.54.jar
fi
