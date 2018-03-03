#!/bin/bash

protoc --cpp_out=./a my.proto
rm *.o my
make
./my
cat ./output.txt
