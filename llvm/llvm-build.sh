#!/bin/bash

rm -rf build
mkdir build
INSTALLDIR=$(realpath $PWD/install)
(
 cd build
SRCDIR=../
cmake \
      -G Ninja \
      "-DCMAKE_BUILD_TYPE=Release" \
      "-DCMAKE_INSTALL_PREFIX=${INSTALLDIR}" \
      "-DLLVM_TARGETS_TO_BUILD=host;X86;ARM;AArch64" \
      "${SRCDIR}"
ninja
ninja install
)
