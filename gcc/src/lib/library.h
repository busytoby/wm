#pragma once
#include <stdlib.h>

enum Category {
    K,
    KVP,
    IO,
    WRITER,
    READER,
    TEXT,
    POPEN
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
struct Library* Pass(struct Library*);
struct Library* Write(struct Book*);

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

struct Book* IOWriter(struct Library* (*callback_writer_function)(struct Book*)) {
    struct Book* M = (struct Book*)malloc(sizeof(struct Book));
    struct Page* C = (struct Page*)malloc(sizeof(struct Page));
    M->i = 0;
    M->h = C;
    C->c = IO;
    C->i = callback_writer_function;
    return M;
}

struct Book* IOReader(struct Book* (*callback_reader_function)(struct Book*)) {
    struct Book* M = (struct Book*)malloc(sizeof(struct Book));
    struct Page* C = (struct Page*)malloc(sizeof(struct Page));
    M->i = 0;
    M->h = C;
    C->c = IO;
    C->i = callback_reader_function;
    return M;
}

struct Library* Key(char* keydata) {
    struct Library* E = L();
    E->h->h->c = K;
    E->h->h->i = keydata;
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

struct Book* TextBook(char *buffer) {
    struct Book* M = (struct Book*)malloc(sizeof(struct Book));
    struct Page* C = (struct Page*)malloc(sizeof(struct Page));
    M->i = TEXT;
    M->h = C;
    C->c = K;
    C->i = buffer;
    return M;
}

struct Book* Launch(char* cmd) {
    struct Book* M = (struct Book*)malloc(sizeof(struct Book));
    struct Page* C = (struct Page*)malloc(sizeof(struct Page));
    M->i = POPEN;
    M->h = C;
    C->c = K;
    C->i = cmd;
    return M;
}
