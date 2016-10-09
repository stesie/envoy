#!/bin/bash

set -e

# Build artifacts
THIRDPARTY_BUILD=/usr/local

# Source artifacts
mkdir thirdparty
cd thirdparty

# GCC 4.9 for everything.
export CC=gcc
export CXX=g++

# openssl
wget https://www.openssl.org/source/openssl-1.0.2h.tar.gz
tar xf openssl-1.0.2h.tar.gz
cd openssl-1.0.2h
./config --prefix=$THIRDPARTY_BUILD -DPURIFY no-shared
make install
cd ..
rm -fr openssl*

# gperftools
wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.5/gperftools-2.5.tar.gz
tar xf gperftools-2.5.tar.gz
cd gperftools-2.5
./configure --prefix=$THIRDPARTY_BUILD --enable-shared=no --enable-frame-pointers
make install
cd ..
rm -fr gperftools*

# gcovr
wget -O gcovr-3.3.tar.gz https://github.com/gcovr/gcovr/archive/3.3.tar.gz
tar xf gcovr-3.3.tar.gz
rm gcovr-3.3.tar.gz


