#!/bin/sh

#This script needs inotify-tools!

BASE_DIR=$(cd $(dirname $0);pwd)
cd ${BASE_DIR}

while inotifywait -e modify ./source; do
    ./make.sh
done
