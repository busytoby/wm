#include "library.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <error.h>

struct Process {   
    FILE* pout;
    struct Process* n;
};

struct Process* HEAD = NULL;
struct Process* TAIL = NULL;

struct Library* Entrance() {
    struct Library* E = Key("POPEN");

    E->h->n = IOWriter(Write);
    E->h->n->i = 1;
    E->h->n->p = E->h;
    E->h->n->n = IOReader(Read);
    E->h->n->n->i = 2;
    E->h->n->n->p = E->h->n;
    return E;
}

struct Library* Write(struct Book* B) {
    if(B->i == -1) {
        fprintf(stderr, "Killing Process Not Yet Implemented");
    } else if(B->i == 5) {
        FILE* pout = popen(B->h->i, "r");
        struct Process* p = (struct Process*)malloc(sizeof(struct Process));
        p->pout = pout;
        p->n = NULL;
        if(HEAD == NULL) HEAD = TAIL = p;
        else if(HEAD->n == NULL) HEAD->n = TAIL = p;
        else if(TAIL->n == NULL) {
            TAIL->n = p;
            TAIL = TAIL->n;
        } else {
            fprintf(stderr, "ERROR: Process Tail Was Not Clear");
        }
        printf("POPEN: %s\n", (char*)B->h->i);
    }
    return NULL;
}

int Signal(int n) {     
    if(n == -1) {
        fprintf(stderr, "Killall not yet implemented\n");
        /*
        *running = false;
        kill(forkpid, SIGKILL);
        waitpid(forkpid, &status, 0);
        if(WTERMSIG(status) == SIGKILL) {
            forkpid = 0;
        }
        pclose(pout);
        */
    } else if(n == 0) {
        char buffer[1024];
        struct Process* p = HEAD;
        while(p != NULL) {
            int pfd = fileno(p->pout);
            int flags = fcntl(pfd, F_GETFL, 0);
            flags |= O_NONBLOCK;
            fcntl(pfd, F_SETFL, flags);

            if(fgets(buffer, sizeof(buffer) - 1, p->pout) != NULL) {
                buffer[sizeof(buffer) - 1] = '\0';
                struct Book* M = (struct Book*)malloc(sizeof(struct Book));
                struct Page* C = (struct Page*)malloc(sizeof(struct Page));
                M->i = 2;
                M->h = C;
                C->c = K;
                C->i = buffer;
                WriteWM(M);
                free(C);
                free(M);
            }
            p = p->n;
        }
    } else {
        /*printf("Received Signal %d\n", n);*/
    }
    return n;
}

struct Book* Read(struct Book* B) {
    printf("Read PHTTPS Test\n");
    if(WriteWM != NULL)
        WriteWM(B);
    return NULL;
}

