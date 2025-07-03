#include "library.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

struct Library* Write(struct Book* B) {
    if(ReadWM != NULL)
        ReadWM(B);
    printf("Write MAIN Test\n");
    return NULL;
}

struct Library* Entrance() {
    struct Library* E = Key("MAIN");
    E->h->n = IOWriter(Write);
    E->h->n->i = 1;
    E->h->n->p = E->h;
    E->h->n->n = IOReader(Read);
    E->h->n->n->i = 2;
    E->h->n->n->p = E->h->n;
    
    return E;
}

struct Book* Read(struct Book* B) {
    printf("Read MAIN Test\n");
    if(WriteWM != NULL)
        WriteWM(B);
    return NULL;
}

struct Library* Pass(struct Library*) {
    fprintf(stderr, "Not Implemented Yet");
    return NULL;
}


