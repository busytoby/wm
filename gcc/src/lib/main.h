#pragma once
#include "library.h"
#include <stdio.h>

typedef struct Book* (*read_callback)(struct Book*);
typedef struct Library* (*write_callback)(struct Book*);

struct Library* Entrance();
read_callback ReadWM;
write_callback WriteWM;
int Signal(int);
struct Book* Read(struct Book*);
struct Library* Pass(struct Library*);
struct Library* Write(struct Book*);

int RegisterWMReader(read_callback r) {
    printf("Registering Reader\n");
    ReadWM = r;
    return 0;
}

int RegisterWMWriter(write_callback w) {
    printf("Registering Writer\n");
    WriteWM = w;
    return 0;
}

