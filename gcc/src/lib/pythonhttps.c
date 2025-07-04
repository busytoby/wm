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

pid_t forkpid;
static bool *running;
FILE* pout;

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
        pclose(pout);
    } else if(n == 1 && forkpid == 0 && !*running) {
        pout = popen("python3.13 /home/mariarahel/src/wm/python/https.py 2>&1", "r");
    } else if(n == 0) {
        char buffer[1024];
        int pfd = fileno(pout);
        int flags = fcntl(pfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        fcntl(pfd, F_SETFL, flags);

        while(!feof(pout)) {
            if(fgets(buffer, sizeof(buffer) - 1, pout) != NULL) {
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
            return 0;
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

