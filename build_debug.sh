#!/bin/bash

libtoolize || { echo "libtoolize failed" 1>&2; exit 1; }
aclocal || { echo "aclocal failed" 1>&2; exit 1; }
autoconf || { echo "autoconf failed" 1>&2; exit 1; }
autoheader || { echo "autoheader failed" 1>&2; exit 1; }
automake --add-missing  || { echo "automake failed" 1>&2; exit 1; }

if [ -d debug ]; then
    rm -rf debug/*
else
    mkdir debug
fi

cd debug
../configure --enable-debug --prefix=/var/tmp/dbg CFLAGS="-ggdb -O0 -Werror -pedantic -Wno-overlength-strings" || { echo "configure failed" 1>&2; exit 1; }
make || { echo "make failed" 1>&2; exit 1; }
