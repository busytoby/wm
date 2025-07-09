#pragma once
#include "library.h"
#include "plugin.h"
#include <stdio.h>

struct Library* Entrance();
read_callback ReadWM;
write_callback WriteWM;

int RegisterWMReader(read_callback r) {
    ReadWM = r;
    return 0;
}

int RegisterWMWriter(write_callback w) {
    WriteWM = w;
    return 0;
}

