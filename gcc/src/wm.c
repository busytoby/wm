#include "lib/library.h"
#include "lib/plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
      bcw("POPEN", POPEN, K, "python3.13 /home/mariarahel/src/wm/python/https.py 2>&1");
      //fprintf(stderr, "HTTPD Launched\n");
    }

    if(argc >= 5) {
      //fprintf(stderr, "Calling POPEN\n");
      bcr("POPEN", POPEN, TEXT, NULL);
      //fprintf(stderr, "POPEN Called\n");
    }
    
    if(argc > 6) {
      bcw("HOT", BIN, BIN, NULL);
    }

    fprintf(stderr, "%d\n", argc);
    if(argc == 15) {
      bcw("POPEN", POPEN, K, "python3.13 /home/mariarahel/src/wm/python/https.py 2>&1");
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