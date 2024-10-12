#!/bin/bash

libtoolize || { echo "libtoolize failed" 1>&2; exit 1; }
aclocal || { echo "aclocal failed" 1>&2; exit 1; }
autoconf || { echo "autoconf failed" 1>&2; exit 1; }
autoheader || { echo "autoheader failed" 1>&2; exit 1; }
automake --add-missing  || { echo "automake failed" 1>&2; exit 1; }

if [ -d release ]; then
    rm -rf release/*
else
    mkdir release
fi

cd release
../configure "$@" || { echo "configure failed" 1>&2; exit 1; }
make || { echo "make failed" 1>&2; exit 1; }
