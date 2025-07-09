#pragma once
#include "library.h"
#include "plugin.h"
#include <stdio.h>

struct Library* Entrance();
struct Library* Write(struct Book* B);
struct Book* Read(struct Book* B);
read_callback ReadWM;
write_callback WriteWM;