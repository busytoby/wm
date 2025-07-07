#include "library.h"
#include "main.h"
#include "plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../bin/01_printTEXT.h"

struct FileWatcher {
    char* f;
    uint fmt;
};

enum CompileType {
    CSOURCE,
};

struct CompileTarget {
    char* f;
    char* d;
    struct CompileTarget* n;
    enum CompileType t;
};

struct CompileTarget* HEAD;
struct CompileTarget* TAIL;

struct CompileTarget* addCompileTarget(char* source, char* target) {
    struct CompileTarget* T = (struct CompileTarget*)malloc(sizeof(struct CompileTarget));
    char* binlib;
    char f[255] = "";
    char d[255] = "";
    binlib = getLibFolder();
    //fprintf(stderr, "CTLib Folder: %s\n", binlib);

    strcat(f, binlib);
    strcat(f, "../../");
    strcat(f, strdup(source));
    T->f = f;
    strcat(d, binlib);
    strcat(d, "");
    strcat(d, strdup(target));
    T->d = d;
    fprintf(stderr, "Compile Target: %s (%s)\n", T->f, T->d);
    T->n = NULL;
    T->t = CSOURCE;
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
    printf("B->i TYPE %lld Not Yet Implemented [\n", B->i);
    printf(B->h->i);
    printf("\n] Write HOT Test\n");
    return NULL;
}

struct Library* Entrance() {
    addCompileTarget("main.c", "main.so.0.0.1");
    addCompileTarget("hot.c", "hot.so.0.0.1");
    addCompileTarget("popen.c", "popen.so.0.0.1");
    return Plugin("HOT");
}

struct Book* Read(struct Book* B) {
    return B;
}

struct Library* Pass(struct Library*) {
    fprintf(stderr, "Not Implemented Yet");
    return NULL;
}


