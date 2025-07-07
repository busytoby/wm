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
  struct Library* L;
  struct EntrancyHandle* n;
};

struct Library* (*library_entrancy_function)();
typedef int* (*register_WMReader_function)(struct Book* (*read_callback)(struct Book*));
typedef int* (*register_WMWriter_function)(struct Library* (*write_callback)(struct Book*));

struct EntrancyHandle* Head;
struct EntrancyHandle* Tail;

struct EntrancyHandle* Enter(struct EntrancyHandle* E, char *f) {
  if(E == NULL) return NULL;
  if(strcmp(E->f, f) == 0) return E;
  if(strcmp(E->L->h->h->i, f) == 0) return E;  
  if(!E->n) return NULL;
  return Enter(E->n, f);
}

char* getLibFolder(char* LibFolder) {
  char WM[255];
  char WMLIB[255] = "/lib";

  readlink("/proc/self/exe", WM, 255);
  for(struct { int i; bool b; } f = { strlen(WM), false }; f.i>=0; f.i--) {
    if(!f.b && WM[f.i] == '/') f.b = true;
    else LibFolder[f.i] = WM[f.i]; 
  }
  strcat(LibFolder, WMLIB);
  return LibFolder;
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

    if(Enter(Head, cwf->d_name) != NULL) continue;

    strcat(fullPath, folder);
    strcat(fullPath, "/");
    strcat(fullPath, cwf->d_name);
 
    if(stat(fullPath, &cws) == -1) {
      fprintf(stderr, "stat failure");
      return NULL;
    }

    if(S_ISREG(cws.st_mode)) {
      printf("Scanning: %s\n", cwf->d_name);
      void *libHandle = dlopen(fullPath, RTLD_LAZY);

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
        printf("Entrancy Library Key: %s\n", (char*)L->h->h->i);
        struct EntrancyHandle* E = (struct EntrancyHandle*)malloc(sizeof(struct EntrancyHandle));
        E->h = libHandle;
        E->f = cwf->d_name;
        E->L = L;
        E->n = NULL;
        if(Head == NULL && strcmp((char*)L->h->h->i, "MAIN") == 0) {
          Head = Tail = E;
        } else if(Head != NULL) {
          Tail->n = E;
          Tail = E;
        }
        register_WMReader_function regreader = (register_WMReader_function) dlsym(libHandle, "RegisterWMReader");
        regreader(Read);
        register_WMWriter_function regwriter = (register_WMWriter_function) dlsym(libHandle, "RegisterWMWriter");
        regwriter(Write);
      } else {
        fprintf(stderr, "Error: Unsupported Library Type\n");
        return NULL;
      }
    }
  }
  closedir(cwd);
    /*
        CallWrite("MAIN", Tail->L->h);
        CallRead("MAIN", Tail->L->h);
    */
  return Head;
}