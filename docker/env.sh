#!/bin/bash
image=ubuntu:16.04
ARGS="--security-opt seccomp=unconfined"
docker run -it --rm $ARGS -w $PWD -v $PWD:$PWD $image
