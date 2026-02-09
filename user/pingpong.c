#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p2c[2];
  int c2p[2];

  pipe(p2c);
  pipe(c2p);

  char buf[1];

  int pid = fork();
  if (pid > 0) { // Parent
    close(p2c[0]);
    close(c2p[1]);

    write(p2c[1], "\n", 1);
    close(p2c[1]);
    if (read(c2p[0], buf, 1) == 1) {
      printf("%d: received pong\n", getpid());
      wait((int *) 0);
      exit(0);
    }
    exit(-1);
  } else if (pid == 0) { // Child
    close(p2c[1]);
    close(c2p[0]);
    
    if (read(p2c[0], buf, 1) == 1) {
      printf("%d: received ping\n", getpid());
      write(c2p[1], "\n", 1);
      close(c2p[1]);
      exit(0);
    }
    exit(-1);
  } else {
    printf("fork error\n");
  }
}