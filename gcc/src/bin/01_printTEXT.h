#include "../lib/library.h"
#include <stdio.h>
#include <stdlib.h>

struct Library* printText_Write(struct Book* B) {
    printf(B->h->i);
    return NULL;
}

struct Book* printText_Read(struct Book* B) {
    printf(B->h->i);
    return NULL;
}
