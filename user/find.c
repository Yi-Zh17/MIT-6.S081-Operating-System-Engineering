#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* findName(char* path) {
  int len = strlen(path);
  char* p;

  for (p = path + len; p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void search(char* path, char* file) {
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_FILE: 
    char* fileName = findName(path);
    if (strcmp(fileName, file) == 0) {
      write(1, path, sizeof(char) * strlen(path));
      char nl = '\n';
      write(1, &nl, 1);
    }
    break;
  case T_DIR:
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 ||
        strcmp(de.name, ".") == 0 ||
        strcmp(de.name, "..") == 0) {
        continue;
      }

      int len = strlen(path) + 1 + strlen(de.name) + 1;
      char* p = (char*)malloc(sizeof(char) * len);
      char slash = '/';
      
      memmove(p, path, strlen(path));
      memmove(p + strlen(path), &slash, 1);
      memmove(p + strlen(path) + 1, de.name, strlen(de.name));
      p[len-1] = '\0';

      search(p, file);
      free(p);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: find <dir> <filename>");
    exit(-1);
  }
  char* path = argv[1];
  char* file = argv[2];

  search(path, file);
  exit(0);
}
