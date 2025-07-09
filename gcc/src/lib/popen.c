#include "library.h"
#include "main.h"
#include "plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <error.h>

struct Process {   
    FILE* pout;
    struct Process* n;
    int pid;
};

struct Process* HEAD = NULL;
struct Process* TAIL = NULL;

struct Library* Entrance() {
    return Plugin("POPEN");
}

struct Library* Write(struct Book* B) {
    if(B->i == -1) {
        fprintf(stderr, "Killing Process Not Yet Implemented");
        /*
        TAIL = HEAD;
        char killcmd[255];
        while(TAIL != NULL) {
            printf("Killing Process %d\n", TAIL->pid);
            //sprintf(killcmd, "kill -KILL %d", TAIL->pid);
            //sprintf(killcmd, "kill -KILL %d", TAIL->pid+1);
            //sprintf(killcmd, "kill -KILL %d", TAIL->pid+2);
            popen(killcmd, "r");
            TAIL = TAIL->n;
            fclose(HEAD->pout);
            free(HEAD);
            HEAD = TAIL;
        }
        */
    } else if(B->i == IO) {
        struct Library* L = (struct Library*)malloc(sizeof(struct Library));
        L->i = IO;
        L->h = B;
        struct Cell* E = (struct Cell*)malloc(sizeof(struct Cell));
        E->f = strdup(L->h->h->c == WRITER ? "Writer" : "Reader");
        E->L = L;
        if(Head == NULL)
          Head = Tail = E;
        else {
            Tail->n = E;
            Tail = E;
        }

        return L;
    } else if(B->i == POPEN) {
        struct Process* p = (struct Process*)malloc(sizeof(struct Process));
        FILE* pout = popen(B->h->i, "r");
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
        int pid = fork();
        if(pid == 0) { 
            p->pid = pid;
            exit(0);
        }
        printf("POPEN: %s\n", (char*)B->h->i);
    } else {
        fprintf(stderr, "Write Type %lld Not Yet Implemented\n", B->i);
        return NULL;
    }
    return NULL;
}

struct Book* Read(struct Book* B) {
    char buffer[1024];
    struct Process* p = HEAD;
    while(p != NULL) {
        int pfd = fileno(p->pout);
        int flags = fcntl(pfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        fcntl(pfd, F_SETFL, flags);

        if(fgets(buffer, sizeof(buffer) - 1, p->pout) != NULL) {
            buffer[sizeof(buffer) - 1] = '\0';
            struct Book* T = Bind(TEXT, K, buffer);
            //fprintf(stderr, "Read\n%s\n", buffer);
            if(Head != NULL) {
//                int count =1;
//                Tail = Head;
//                while(Tail->n != NULL) { count++; Tail = Tail->n; }
//                fprintf(stderr, "%d %d %lld %d %s\n", count, Head->L->i, Head->L->h->i, Head->L->h->h->c, Head->f);
//                fprintf(stderr, "%d %d %lld %d %s\n", count, Head->n->L->i, Head->n->L->h->i, Head->n->L->h->h->c, Head->n->f);
                struct Cell* E = Enter(Head, "Writer");
                write_callback writer = (write_callback)(E->L->h->h->i);
                writer(T);
            } else {
                fprintf(stderr, "Head Null\n");
            }
            //WriteWM(T);
            free(T->h);
            free(T);
        }
        p = p->n;
    }
    return NULL;
}

