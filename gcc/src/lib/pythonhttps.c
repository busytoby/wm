#include "library.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <error.h>

pid_t forkpid;
static bool *running;

struct Library* Entrance() {
    struct Library* E = Key("PHTTPS");
    forkpid = 0;
    running = mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *running = false;

    E->h->n = IOWriter(Write);
    E->h->n->i = 1;
    E->h->n->p = E->h;
    E->h->n->n = IOReader(Read);
    E->h->n->n->i = 2;
    E->h->n->n->p = E->h->n;
    return E;
}

struct Library* Write(struct Book* B) {
    if(ReadWM != NULL)
        ReadWM(B);
    printf("Write PHTTPS Test\n");
    return NULL;
}

int Signal(int n) {   
    int status;
   
    if(n == -1 && forkpid != 0 && *running) {
        *running = false;
        kill(forkpid, SIGKILL);
        waitpid(forkpid, &status, 0);
        if(WTERMSIG(status) == SIGKILL) {
            forkpid = 0;
        }
    } else if(n == 1 && forkpid == 0 && !*running) {
        static char server[] = "../../python/https.py";
        char *args[] = { "python3.13", server, NULL, };
        pid_t const r = fork();
        if(r == -1) {
            fprintf(stderr, "Fork Error\n");
            return -1;
        } else if(r == 0) {
            execvp("python3.13", args);
            return 0;
        } else {
            forkpid = r;
            *running = true;
            return r;
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

