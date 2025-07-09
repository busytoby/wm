#pragma once
#include <stdlib.h>

enum Category {
    K,
    KVP,
    IO,
    WRITER,
    READER,
    TEXT,
    BIN,
    POPEN,
};

struct Page {
    void* i;
    enum Category c;
};

struct Book {
    long long i;
    struct Page* h;
    struct Book* n;
    struct Book* p;
};

struct Library {
    char i;
    struct Book* h;
};

struct Book* Read(struct Book*);
struct Library* Write(struct Book*);

struct Book* Bind(enum Category M, enum Category C, void* ptr) {
    struct Book* B = (struct Book*)malloc(sizeof(struct Book));
    struct Page* P = (struct Page*)malloc(sizeof(struct Page));
    B->i = M;
    B->h = P;
    P->c = C;
    P->i = ptr;
    return B;
}

struct Book* IOWriter(struct Library* (*callback_writer_function)(struct Book*)) {
    return Bind(WRITER, IO, callback_writer_function);
}

struct Book* IOReader(struct Book* (*callback_reader_function)(struct Book*)) {
    return Bind(READER, IO, callback_reader_function);
}

struct Library* L() {
    struct Library* E = (struct Library*)malloc(sizeof(struct Library));
    struct Book* M = (struct Book*)malloc(sizeof(struct Book));
    struct Page* C = (struct Page*)malloc(sizeof(struct Page));
    E->i = 0;
    M->i = 0;
    M->h = C;
    E->h = M;
    return E;
}

struct Library* Key(void* key) {
    struct Library* E = L();
    E->h->h->c = K;
    E->h->h->i = key;
    return E;
}

struct Library* Plugin(char *name) {
    struct Library* L = Key(name);
    L->h->n = IOWriter(Write);
    L->h->n->i = WRITER;
    L->h->n->p = L->h;
    L->h->n->n = IOReader(Read);
    L->h->n->n->i = READER;
    L->h->n->n->p = L->h->n;
    return L;
}