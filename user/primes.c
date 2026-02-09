#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primeFilter(int) __attribute__((noreturn));

void
primeFilter(int inputFd) {
  int p; // prime
  int buf; // Other numbers

  int newPipe[2];


  read(inputFd, &p, 4);
  printf("prime %d\n", p);

  while (read(inputFd, &buf, 4) != 0) { // Fork a process on first non-divisable number
    if (buf % p != 0) {
      pipe(newPipe);
      
      int pid = fork();
      if (pid == 0) {
        close(newPipe[1]);
        close(inputFd);
        primeFilter(newPipe[0]);
      }
      else if (pid > 0) {
        close(newPipe[0]);
        write(newPipe[1], &buf, 4);
      }
      else {
        printf("Fork error");
      }
      break;
    }
  }

  while (read(inputFd, &buf, 4) != 0) {
    if (buf % p != 0) {
      write(newPipe[1], &buf, 4);
    }
  }
  close(newPipe[1]);
  wait(0);
  exit(0);
}

int
main(int argc, char* argv[]) {
  int genPipe[2];
  pipe(genPipe);

  int buf = 0;

  int pid = fork();

  if (pid == 0) {
    close(genPipe[1]);
    primeFilter(genPipe[0]);
  }
  else if (pid > 0) {
    close(genPipe[0]);

    for (int i = 2; i <= 35; ++i) {
      buf = i;
      write(genPipe[1], &buf, 4);
    }
    
    close(genPipe[1]);
  }
  else {
    printf("Fork error");
  }
  wait(0);
  exit(0);
}
