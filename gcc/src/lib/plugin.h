#pragma once
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

struct EntrancyHandle {
  void *h;
  char *f;
  char *p;
  struct Library* L;
  struct EntrancyHandle* n;
  time_t m;
};

struct Library* (*library_entrancy_function)();
typedef struct Book* (*read_callback)(struct Book*);
typedef struct Library* (*write_callback)(struct Book*);

typedef struct Book* (*register_WMReader_function)(struct Book* (*read_callback)(struct Book*));
typedef struct Library* (*register_WMWriter_function)(struct Library* (*write_callback)(struct Book*));

struct EntrancyHandle* Head;
struct EntrancyHandle* Tail;

struct EntrancyHandle* Enter(struct EntrancyHandle* E, char* f) {
  if(E == NULL) return NULL;
  if(strcmp(E->f, f) == 0) return E;
  if(strcmp(E->p, f) == 0) return E;
  if(strcmp(E->L->h->h->i, f) == 0) return E;  
  if(!E->n) return NULL;
  return Enter(E->n, f);
}

struct Library* CallWrite(char* Key, struct Book* B) {
    //fprintf(stderr, "CallWrite\n");
    struct EntrancyHandle* E = Enter(Head, Key);
    if(E == NULL) {
      fprintf(stderr, "No [%s] Plugin Found\n", Key);
      return NULL;
    }
    if(E->h == NULL) {
      fprintf(stderr, "Cannot Write To Closed DLL\n");
      return NULL;
    }
    write_callback writer = (write_callback)(E->L->h->n->h->i);
    return writer(B);
}

struct Book* CallRead(char* Key, struct Book* B) {
    //fprintf(stderr, "CallRead\n");
    struct EntrancyHandle* E = Enter(Head, Key);
    if(E == NULL) {
      fprintf(stderr, "No [%s] Plugin Found\n", Key);
      return NULL;
    }
    if(E->h == NULL) {
      fprintf(stderr, "Cannot Read From Closed DLL\n");
      return NULL;
    }
    read_callback reader = (read_callback)(E->L->h->n->n->h->i);
    struct Book* A = reader(B);
    if(A == NULL) return A;
    if(A->i == K) {
      printf("Read K: %s\n", (char *)A->h->i);
    } else {
      fprintf(stderr, "No Read Handler Implemented For Type %lld", A->i);
    }
    return A;
}

struct Library* bcw(char* Key, enum Category M, enum Category C, void* ptr) {
    struct Book* P = Bind(M, C, ptr);
    struct Library* A = CallWrite(Key, P);
    if(A == NULL) {
        free(P->h);
        free(P);
    }
    return A;
}

struct Book* bcr(char* Key, enum Category M, enum Category C, void* ptr) {
    struct Book* P = Bind(M, C, ptr);
    struct Book* B = CallRead(Key, P);
    if(B == NULL) {
        free(P->h);
        free(P);
    }
    return B;
}

char* getLibFolder() {
  char WM[255];
  char WMLIB[255] = "/lib";
  char LibFolder[255] = "";

  readlink("/proc/self/exe", WM, 255);
  for(struct { int i; bool b; } f = { strlen(WM), false }; f.i>=0; f.i--) {
    if(!f.b && WM[f.i] == '/') f.b = true;
    else LibFolder[f.i] = WM[f.i]; 
  }
  strcat(LibFolder, WMLIB);
  strcat(LibFolder, "/");
  //fprintf(stderr, "Lib Folder: %s\n", LibFolder);
  return strdup(LibFolder);
}

struct EntrancyHandle* unloadPlugin(char* name) {
    struct EntrancyHandle* E = Enter(Head, name);
    if(E == NULL) return Head;
    if(E->h == NULL) return E;
    bcw(E->f, BIN, BIN, NULL);
    fprintf(stderr, "Unload Plugin %s", name);
    dlclose(E->h);
    E->h = NULL;
    fprintf(stderr, " Done !\n");    
    return E;
}

struct EntrancyHandle* scanLib(char* arg, char* folder) {
  DIR* cwd;
  struct dirent *cwf;
  cwd = opendir(folder);

  if(arg != NULL) {
    fprintf(stderr, "Scanning For Non-NULL Specifics Not Yet Implemented");
    return NULL;
  }

  if(cwd == NULL) {
      fprintf(stderr, "No Library Folder Found At %s\n", folder);
      return NULL;
  }

  while((cwf = readdir(cwd))) {
    struct stat cws;
    char fullPath[255] = "";
    struct EntrancyHandle *E;

    //int c = 0;
    //E=Head;
    //while(E != NULL) { fprintf(stderr, "elib %s (%s)\n", E->f, E->p); E = E->n; c++; }
    //fprintf(stderr, "LibCount: %d\n", c);

    E = Enter(Head, cwf->d_name);
       
    strcat(fullPath, folder);
    strcat(fullPath, cwf->d_name);
 
    if(stat(fullPath, &cws) == -1) {
      fprintf(stderr, "stat failure");
      return NULL;
    }

    if(E != NULL) {
      stat(fullPath, &cws);
      if(E->m != cws.st_mtime) {
        E = unloadPlugin(E->f);
        fprintf(stderr, "Change Detected %s [%s]\n", E->f, E->p);
      }
      if (E->h != NULL) continue;
      //else fprintf(stderr, "Reloading %s\n", cwf->d_name);
    }

    //if(E != NULL && E->h == NULL) fprintf(stderr, "Detected Unloaded Library %s\n", E->f);

    if(S_ISREG(cws.st_mode)) {
      void *libHandle;

      if(E == NULL || E->h == NULL) {
        printf("Scanning: %s\n", fullPath);
        if(E == NULL) E = (struct EntrancyHandle*)malloc(sizeof(struct EntrancyHandle));
      }

      //fprintf(stderr, "ehnull: %b\n", (E->h == NULL));
      libHandle = (E->h != NULL) ? E->h : dlopen(fullPath, RTLD_LAZY);

      if(!libHandle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return NULL;
      }

      *(struct Library**)(&library_entrancy_function) = dlsym(libHandle, "Entrance");
      if(!library_entrancy_function) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(libHandle);
        return NULL;
      }

      struct Library* L = library_entrancy_function();
      if(L->h->h->c == K) {
        struct stat attr;
        printf("Entrancy Library Key: %s\n", (char*)L->h->h->i);
        if(E == NULL) E = (struct EntrancyHandle*)malloc(sizeof(struct EntrancyHandle));
        E->h = libHandle;
        if(E->f != NULL) free(E->f);
        E->f = strdup(cwf->d_name);
        if(E->p != NULL) free(E->p);
        E->p = strdup(fullPath);
        E->L = L;
        stat(E->p, &attr);
        E->m = attr.st_mtime;

        if(Head == NULL && strcmp((char*)L->h->h->i, "MAIN") == 0) {
          Head = Tail = E;
        } else if(Head != NULL) {
          bool found = false;
          Tail = Head;
          if(Tail == E) found = true;
          while(Tail->n != NULL) {
            if(Tail->n == E) found = true;
            Tail = Tail->n;
          }
          if(!found) {
            Tail->n = E;
            Tail = E;
          }
        }

        bcw(E->f, IO, WRITER, Write);
        bcw(E->f, IO, READER, Read);

        fprintf(stderr, "Stored %s (%s)\n", E->f, E->p);
      } else {
        fprintf(stderr, "Error: Unsupported Library Type\n");
        return NULL;
      }
    }
  }
  closedir(cwd);

  //int c = 0;
  //struct EntrancyHandle* E=Head;
  //while(E != NULL) { fprintf(stderr, "final elib %s (%s)\n", E->f, E->p); E = E->n; c++; }
  //fprintf(stderr, "Final LibCount: %d\n", c);

  return Head;
}