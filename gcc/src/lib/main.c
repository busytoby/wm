#include "library.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "../bin/01_printTEXT.h"

struct KVP {
    struct Book* k;
    struct Book* v;
    struct KVP* n;
};

struct KVP* HEAD;
struct KVP* TAIL;

struct KVP* FK(struct KVP* Start, struct Book* K) {
    if(Start == NULL) return NULL;    
    if(Start->k->i == K->i && Start->k->h->c == K->h->c) return HEAD;
    return FK(Start->n, K);
}
struct KVP* FINDK(struct Book* K) { return FK(HEAD, K); }

struct Library* Write(struct Book* B) {
    if(B->i == K) {
        struct KVP* I = FINDK(B);
        if(I == NULL) {
            struct KVP* N = (struct KVP*)malloc(sizeof(struct KVP));
            N->k = B;
            if(HEAD == NULL) HEAD = TAIL = N;
            else if(HEAD->n == NULL) HEAD->n = TAIL = N;
            else if(TAIL->n == NULL) {
                TAIL->n = N;
                TAIL = N;
            }
            else fprintf(stderr, "KVP Tail Is Closed");
        } else {
            I->k = B;
            I->v = NULL;
        } 
        return NULL;
    }
    if(B->i == IO) {
        //printf("IO\n");
        struct Library* L = (struct Library*)malloc(sizeof(struct Library));
        L->i = IO;
        L->h = B;
        if(L->h->h->c == WRITER) {
            //fprintf(stderr, "Storing WMWriter\n");
            //write_callback writer = (write_callback)(L->h->h->i);
            //writer(B);
        } else if(L->h->h->c == READER) {
            //fprintf(stderr, "Storing WMReader\n");
            //read_callback reader = (read_callback)(L->h->h->i);
            //reader(B);
        } else {
            //fprintf(stderr, "IO Type Unknown\n");
            return NULL;
        }
        struct EntrancyHandle* E = (struct EntrancyHandle*)malloc(sizeof(struct EntrancyHandle));
        E->f = strdup(L->h->h->c == WRITER ? "Writer" : "Reader");
        E->L = L;
        if(Head == NULL)
          Head = Tail = E;
        else {
            Tail->n = E;
            Tail = E;
        }

        //fprintf(stderr, "Stored IO\n");
        return NULL;
    }
    if(B->i == TEXT) {
        printText_Write(B);
    } else {
    printf("B->i TYPE %lld Not Yet Implemented [\n", B->i);
        printf(B->h->i);
    printf("\n] Write MAIN Test\n");
    }
    return NULL;
}

struct Library* Entrance() {
    return Plugin("MAIN");
}

struct Book* Read(struct Book* B) {
    struct KVP* I = FINDK(B);
    if(I == NULL) return NULL;
    if(I->v == NULL) return I->k;
    return I->v;
}
