#include "lib/library.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

struct EntrancyHandle* Head = NULL;
struct EntrancyHandle* Tail = NULL;

struct Library* (*library_entrancy_function)();
typedef struct Library* (*write_callback)(struct Book*);
typedef struct Book* (*read_callback)(struct Book*);

struct Library* Write(struct Book* B) {
    printf("Write WM Test\n");
    return NULL;
}

struct Book* Read(struct Book* B) {
    printf("Read WM Test\n");
    return NULL;
}

struct EntrancyHandle {
  void *h;
  char *f;
  struct Library* L;
  struct EntrancyHandle* n;
};

struct EntrancyHandle* Enter(struct EntrancyHandle* E, char *f) {
  if(E == NULL) return NULL;
  if(strcmp(E->f, f) == 0) return E;
  if(strcmp(E->L->h->h->i, f) == 0) return E;  
  if(!E->n) return NULL;
  return Enter(E->n, f);
}

int CallSignal(char *Key, int Signal) {
    typedef int* (*signal_fptr)(int);
    struct EntrancyHandle* E = Enter(Head, Key);
    if(E == NULL) {
      fprintf(stderr, "No [%s] Plugin Found\n", Key);
      return EXIT_FAILURE;
    }

    signal_fptr signal = (signal_fptr) dlsym(E->h, "Signal");
    if(!signal) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(E->h);
        return EXIT_FAILURE;
    }
    return (int)signal(Signal);
}

int main(int argc, char** argv) {
  int EntranceCount = 0;
  DIR* cwd;
  struct dirent* cwf;
  char LIB[6] = "./lib";

  typedef int* (*register_WMReader_function)(struct Book* (*read_callback)(struct Book*));
  typedef int* (*register_WMWriter_function)(struct Library* (*write_callback)(struct Book*));

  if (argc > 1) {
    fprintf(stderr, "Arguments Not Supported");
    return EXIT_FAILURE;
  }

  while(1) {
    cwd = opendir(LIB);
    if(cwd == NULL) {
      fprintf(stderr, "No Library Folder Found");
      return EXIT_FAILURE;
    }

    while((cwf = readdir(cwd))) {
      struct stat cws;
      char fullPath[40] = "";

      if(Enter(Head, cwf->d_name) != NULL) continue;

      strcat(fullPath, LIB);
      strcat(fullPath, "/");
      strcat(fullPath, cwf->d_name);
 
      if(stat(fullPath, &cws) == -1) {
        fprintf(stderr, "stat failure");
        return EXIT_FAILURE;
      }
      if(S_ISREG(cws.st_mode)) {
        printf("Potential Library: %s\n", cwf->d_name);
        void *libHandle = dlopen(fullPath, RTLD_LAZY);

	      if(!libHandle) {
          fprintf(stderr, "Error: %s\n", dlerror());
          return EXIT_FAILURE;
        }

        *(struct Library**)(&library_entrancy_function) = dlsym(libHandle, "Entrance");
        if(!library_entrancy_function) {
          fprintf(stderr, "Error: %s\n", dlerror());
          dlclose(libHandle);
          return EXIT_FAILURE;
        }

        struct Library* L = library_entrancy_function();
        if(L->h->h->c == K) {
          printf("Entrancy Library Key: %s\n", (char*)L->h->h->i);
          if(Head == NULL && strcmp((char*)L->h->h->i, "MAIN") == 0) {
            Head = (struct EntrancyHandle*)malloc(sizeof(struct EntrancyHandle));
            Head->h = libHandle;
            Head->f = cwf->d_name;
            Head->L = L;
            Head->n = NULL;
            Tail = Head;
            write_callback writer = (write_callback)(Head->L->h->n->h->i);
            read_callback reader = (read_callback)(Head->L->h->n->n->h->i);
            writer(Head->L->h);
            reader(Head->L->h);
            register_WMReader_function regreader = (register_WMReader_function) dlsym(libHandle, "RegisterWMReader");
            regreader(Read);
            register_WMWriter_function regwriter = (register_WMWriter_function) dlsym(libHandle, "RegisterWMWriter");
            regwriter(Write);
            writer(Head->L->h);
            reader(Head->L->h);
            EntranceCount++;
          } else if(Head != NULL) {
            struct EntrancyHandle* E = (struct EntrancyHandle*)malloc(sizeof(struct EntrancyHandle));
            E->h = libHandle;
            E->f = cwf->d_name;
            E->L = L;
            E->n = NULL;
            Tail->n = E;
            Tail = E;
            write_callback writer = (write_callback)(Tail->L->h->n->h->i);
            read_callback reader = (read_callback)(Tail->L->h->n->n->h->i);
            writer(Tail->L->h);
            reader(Tail->L->h);
            register_WMReader_function regreader = (register_WMReader_function) dlsym(libHandle, "RegisterWMReader");
            regreader(Read);
            register_WMWriter_function regwriter = (register_WMWriter_function) dlsym(libHandle, "RegisterWMWriter");
            regwriter(Write);
            writer(Head->L->h);
            reader(Head->L->h);
            EntranceCount++;
          }
        } else {
          fprintf(stderr, "Error: Unsupported Library Type\n");
          return EXIT_FAILURE;
        }
        /*dlclose(libHandle);*/
      } else {
/*        printf("Irregular Library: %s\n", cwf->d_name); */
      }
    }

    usleep(1000000);
    argc++;
    if(argc == 4) {
      CallSignal("PHTTPS", 1);
    }
  }

  printf("Finished\n");
  closedir(cwd);
  return 0;
}