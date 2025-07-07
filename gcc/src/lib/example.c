#include "library.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "../bin/01_printTEXT.h"

struct Library* Write(struct Book* B) {
    printf("B->i TYPE %lld Not Yet Implemented [\n", B->i);
    printf(B->h->i);
    printf("\n] Write EXAMPLE Test\n");
    return NULL;
}

struct Library* Entrance() {
    return Plugin("EXAMPLE");
}

struct Book* Read(struct Book* B) {
    return B;
}

struct Library* Pass(struct Library*) {
    fprintf(stderr, "Not Implemented Yet");
    return NULL;
}


