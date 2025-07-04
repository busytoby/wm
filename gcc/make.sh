#!/bin/bash 

CFLAGS="-Wall"
LDFLAGS="-ldl -lc -I/usr/include/python3.13 -I/usr/include/x86_64-linux-gnu/python3.13 -lm"

gcc -g -shared -fPIC $CFLAGS -o bin/lib/main.so.0.0.1 src/lib/main.c
gcc -g -shared -fPIC $CFLAGS -o bin/lib/popen.so.0.0.1 src/lib/popen.c
#gcc -g -shared -fPIC $CFLAGS -I/usr/include/python3.13 -I/usr/include/x86_64-linux-gnu/python3.13 -fno-strict-overflow -Wsign-compare -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions -DNDEBUG -g -O2 -o bin/lib/pythonhttps.so.0.0.1 src/lib/pythonhttps.c -lpython3.13 -lz -lexpat
gcc -g $CFLAGS -o bin/wm src/wm.c $LDFLAGS

