#!/bin/bash 

CFLAGS="-Wall"
LDFLAGS="-ldl -lc"

gcc -g -shared -fPIC $CFLAGS -o bin/lib/main.so.0.0.1 src/lib/main.c
gcc -g -shared -fPIC $CFLAGS -o bin/lib/popen.so.0.0.1 src/lib/popen.c
gcc -g $CFLAGS -o bin/wm src/wm.c $LDFLAGS

