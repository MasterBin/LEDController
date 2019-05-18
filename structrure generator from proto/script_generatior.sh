#!/bin/bash

if [ $# != 1 ]; then
    echo "Enter name of proto file!"
    exit
fi

BASEDIR=$(dirname "$0")

proto_path=$1
# echo proto_path: $proto_path
proto_dir=${proto_path%\/*}
# echo proto_dir: $proto_dir
name=$(basename "$proto_path" ".proto")
# echo name: $name
pb_path=$proto_dir"/"$name".pb"
# echo pb_path: $pb_path

# protoc -o $pb_path -I $proto_dir $proto_path
protoc -o $pb_path $proto_path
python $BASEDIR"/../lib/nanopb-master/generator/nanopb_generator.py" $pb_path