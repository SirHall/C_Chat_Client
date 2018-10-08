#!/bin/bash
#Who even uses makefiles anyway? :D
objOut="./object/"
exec="./bin/server"
libraries="-lncurses -lpthread"
libs=""
flags="-Wall -Wno-unknown-pragmas"
include="-I./include/server -I./include/both \
-I/usr/include -I/usr/local/include -I./**"
CPPVersion="c++17"

#Find uncompiled files (.c)
raws=$(find ./src/server/ -name "*.c"; find ./src/both/ -name "*.c")
for i in $raws
    do echo $i
done

gcc $flags -o $exec $raws $libs $libraries $include
