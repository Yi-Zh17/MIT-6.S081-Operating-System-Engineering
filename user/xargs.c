#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
  char* command = argv[1];

  char** nArgv = (char**) malloc(sizeof(char*) * MAXARG);
  nArgv[0] = argv[1];
  nArgv[1] = argv[2];
  nArgv[3] = 0;

  char buf;
  char arg[MAXARG];
  int i = 0;
  while(read(0, &buf, 1) == 1 && i < MAXARG - 1) {
    if (buf == '\n') {
      arg[i] = '\0';
      nArgv[2] = arg;
      int pid = fork();
      if (pid == 0) {
        exec(command, nArgv);
        exit(0);
      } else if (pid > 0) {
        wait(0);
        memset(arg, 0, sizeof(arg));
        i = 0;
      } else {
        printf("Fork error");
      }
    } else {
      arg[i] = buf;
      ++i;
    }
  }

  if (arg[0] != '\0') {
    arg[i] = '\0';
    nArgv[2] = arg;
    exec(command, nArgv);
  }

  free(nArgv);

  exit(0);
  
}