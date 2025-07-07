#pragma once
#include "library.h"
#include <stdio.h>

typedef struct Book* (*read_callback)(struct Book*);
typedef struct Library* (*write_callback)(struct Book*);

struct Library* Entrance();
read_callback ReadWM;
write_callback WriteWM;
int Signal(int);

int RegisterWMReader(read_callback r) {
    ReadWM = r;
    return 0;
}

int RegisterWMWriter(write_callback w) {
    WriteWM = w;
    return 0;
}

