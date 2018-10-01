#!/bin/bash

build_cpp()
{
protoc --cpp_out=./a my.proto
rm *.o my
make
./my
cat ./output.txt
}

build_py()
{
protoc --python_out=./a my.proto
}
build_py
