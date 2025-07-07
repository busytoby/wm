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
    char* f;
    char* d;
    struct CompileTarget* n;
    enum CompileType t;
    time_t m;
};

struct CompileTarget* HEAD;
struct CompileTarget* TAIL;

struct CompileTarget* addCompileTarget(char* source, char* target) {
    struct CompileTarget* T = (struct CompileTarget*)malloc(sizeof(struct CompileTarget));
    char* binlib;
    char f[255] = "";
    char d[255] = "";
    struct stat attr;
    binlib = getLibFolder();
    //fprintf(stderr, "CTLib Folder: %s\n", binlib);

    sprintf(f, "%s../../src/lib/%s", binlib, source);
    T->f = strdup(f);
    sprintf(d, "%s%s", binlib, target);
    T->d = strdup(d);
    T->n = NULL;
    stat(T->f, &attr);
    T->m = attr.st_mtime;
    T->t = CSOURCE;
    fprintf(stderr, "Compile Target: %s (%s) [%ld]\n", T->f, T->d, T->m);
    if(HEAD == NULL) HEAD = TAIL = T;
    else if(HEAD->n == NULL || TAIL == NULL) HEAD->n = TAIL = T;
    else {
        TAIL->n = T;
        TAIL = T;
    }
    free(binlib);
    return HEAD;
}

int Signal(int n) {    
    printf("Received Signal %d\n", n);    
    if(n == 1) {
        struct CompileTarget* T = HEAD;
        struct stat attr;
        while(T != NULL) {                
            stat(T->f, &attr);
            printf("%ld // %ld\n", attr.st_mtime, T->m);
            T = T->n;
        }
    } else {
        /*printf("Received Signal %d\n", n);*/
    }
    return n;
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


