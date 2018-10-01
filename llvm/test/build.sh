#!/bin/bash

OPT=opt-3.8
OPT=/media/code/git/llvm-master/install/bin/opt
clang --target=aarch64 -S -emit-llvm test.c
clang --target=aarch64 -c -emit-llvm test.c
$OPT -analyze -targetlibinfo -cost-model ./test.bc
$OPT -load libhello.so -hello <  ../test/test.bc
$OPT -load libhello.so -hello2 <  ../test/test.bc
