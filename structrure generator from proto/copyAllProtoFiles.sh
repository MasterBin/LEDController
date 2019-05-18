#!/bin/bash

if [ $# != 1 ]; then
    echo "Enter path!"
    exit
fi

BASEDIR=$(dirname "$0")
directory=$1


for file in `find $BASEDIR"/../lib/Modes" -type f -name "*.proto"`; do
    cp $file $directory
done

echo "Copying complete!";