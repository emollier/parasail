#!/bin/bash
set -e -x

name=parasail-`./version.sh`-manylinux1_$SUFFIX
./configure --prefix=`pwd`/$name
make -j 2
make install
tar -cvzf $name.tar.gz $name
mkdir -p dist
cp $name.tar.gz dist
