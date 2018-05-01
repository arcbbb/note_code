#!/bin/bash
clang --target=aarch64 -S -emit-llvm test.c
clang --target=aarch64 -c -emit-llvm test.c
opt-3.8 -analyze -targetlibinfo -cost-model ./test.bc

