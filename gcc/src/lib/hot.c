#include "library.h"
#include "main.h"
#include "plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include "../bin/01_printTEXT.h"

enum CompileType {
    CSOURCE,
};

struct CompileTarget {
    char* i;
    char* f;
    char* d;
    struct CompileTarget* n;
    enum CompileType t;
    time_t m;
};

struct CompileTarget* HEAD;
struct CompileTarget* TAIL;

struct CompileTarget* addCompileTarget(char* identifier, char* source, char* target) {
    struct CompileTarget* T = (struct CompileTarget*)malloc(sizeof(struct CompileTarget));
    char* binlib;
    char f[255] = "";
    char d[255] = "";
    struct stat attr;
    binlib = getLibFolder();
    //fprintf(stderr, "CTLib Folder: %s\n", binlib);

    T->i = strdup(identifier);
    sprintf(f, "%s../../src/lib/%s", binlib, source);
    T->f = strdup(f);
    sprintf(d, "%s%s", binlib, target);
    T->d = strdup(d);
    T->n = NULL;
    stat(T->f, &attr);
    T->m = attr.st_mtime;
    T->t = CSOURCE;
    fprintf(stderr, "Compile Target: %s %s (%s) [%ld]\n", T->i, T->f, T->d, T->m);
    if(HEAD == NULL) HEAD = TAIL = T;
    else if(HEAD->n == NULL || TAIL == NULL) HEAD->n = TAIL = T;
    else {
        TAIL->n = T;
        TAIL = T;
    }
    free(binlib);
    return HEAD;
}

struct Library* Write(struct Book* B) {
    if(B->i == BIN) {
        struct CompileTarget* T = HEAD;
        struct stat attr;
        char gcc[255];
        while(T != NULL) {                
            stat(T->f, &attr);
            if(attr.st_mtime > T->m) {
                fprintf(stderr, "Detected Change In %s (%s)\n", T->d, T->f);
                //gcc -g -shared -fPIC $CFLAGS -o bin/lib/main.so.0.0.1 src/lib/main.c
                remove(T->d);
                sprintf(gcc, "gcc -g -shared -fPIC -Wall -o %s %s", T->d, T->f);;               
                popen(gcc, "r");
                T->m = attr.st_mtime;
            }
            T = T->n;
        }
    } else {
    printf("B->i TYPE %lld Not Yet Implemented [\n", B->i);
    printf(B->h->i);
    printf("\n] Write HOT Test\n");
    }
    return NULL;
}

struct Library* Entrance() {
    addCompileTarget("MAIN", "main.c", "main.so.0.0.1");
    addCompileTarget("HOT", "hot.c", "hot.so.0.0.1");
    addCompileTarget("POPEN", "popen.c", "popen.so.0.0.1");
    return Plugin("HOT");
}

struct Book* Read(struct Book* B) {
    return B;
}
