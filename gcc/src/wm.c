#include "lib/library.h"
#include "lib/plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Library* (*write_callback)(struct Book*);
typedef struct Book* (*read_callback)(struct Book*);
typedef int (*signal_fptr)(int);

int CallWrite(char* Key, struct Book* B) {
    //fprintf(stderr, "CallWrite\n");
    struct EntrancyHandle* E = Enter(Head, Key);
    if(E == NULL) {
      fprintf(stderr, "No [%s] Plugin Found\n", Key);
      return EXIT_FAILURE;
    }
    if(E->h == NULL) {
      fprintf(stderr, "Cannot Write To Closed DLL\n");
      return EXIT_FAILURE;
    }
    write_callback writer = (write_callback)(E->L->h->n->h->i);
    writer(B);
    return 0;
}

int CallRead(char* Key, struct Book* B) {
    //fprintf(stderr, "CallRead\n");
    struct EntrancyHandle* E = Enter(Head, Key);
    if(E == NULL) {
      fprintf(stderr, "No [%s] Plugin Found\n", Key);
      return EXIT_FAILURE;
    }
    if(E->h == NULL) {
      fprintf(stderr, "Cannot Read From Closed DLL\n");
      return EXIT_FAILURE;
    }
    read_callback reader = (read_callback)(E->L->h->n->n->h->i);
    struct Book* A = reader(B);
    if(A == NULL) return 0;
    if(A->i == K) {
      printf("Read K: %s\n", (char *)A->h->i);
    } else {
      fprintf(stderr, "No Read Handler Implemented For Type %lld", A->i);
    }
    return 0;
}

int CallSignal(char *Key, int Signal) {
    struct EntrancyHandle* E = Enter(Head, Key);
    if(E == NULL) {
      fprintf(stderr, "No [%s] Plugin Found\n", Key);
      return EXIT_FAILURE;
    }

    if(E->h == NULL) {
      fprintf(stderr, "Cannot Signal To Closed DLL\n");
      return EXIT_FAILURE;
    }

    signal_fptr signal = (signal_fptr) dlsym(E->h, "Signal");
    if(!signal) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(E->h);
        return EXIT_FAILURE;
    }
    return signal(Signal);
}

int main(int argc, char** argv) {
  char* WMLIBFOLDER = getLibFolder();
  printf("Folder: %s\n", WMLIBFOLDER);

  if (argc > 1) {
    fprintf(stderr, "Arguments Not Supported\n");
    return EXIT_FAILURE;
  }

  /* Pass One */
  struct EntrancyHandle* E = scanLib(NULL, WMLIBFOLDER);
  if(E == NULL) {
    fprintf(stderr, "No Libraries Found\n");
    return EXIT_FAILURE;
  }

  do {
    /* Pass Two */
    /*
    while((E = scanLib(overt, WMLIBFOLDER)) != NULL) {
      Do Stuff
    }
    */

    /* Pass Three */
    /* Do Whatever */
    usleep(1000000);
    argc++;
    if(argc == 4) {
      //fprintf(stderr, "Launching HTTPD\n");
      struct Book* P = Launch("python3.13 /home/mariarahel/src/wm/python/https.py 2>&1");
      CallWrite("POPEN", P);
      free(P->h);
      free(P);
      //fprintf(stderr, "HTTPD Launched\n");
    }

    if(argc >= 5) {
      //fprintf(stderr, "Calling POPEN\n");
      CallSignal("POPEN", 0);
      //fprintf(stderr, "POPEN Called\n");
    }
    
    if(argc > 6) {
      CallSignal("HOT", 1);
    }
  } while((E = scanLib(NULL, WMLIBFOLDER)) != NULL);

  printf("Finished\n");
  return 0;
}

struct Library* Write(struct Book* B) {
    printf("wWM [\n");
    CallWrite("MAIN", B);
    printf("]\n");
    return NULL;
}

struct Book* Read(struct Book* B) {
    printf("rWM [\n");
    printf("%s", (char*)B->h->i);
    printf("]\n");
    return NULL;
}