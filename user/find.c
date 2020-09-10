#include "kernel/types.h"

#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

int find(char *dir, char *file);

int main(int argc, char *argv[]) {

  if (argc != 3) {
    fprintf(2, "Usage: find [dir] [file]\n");
    exit(0);
  }

  find(argv[1], argv[2]);

  exit(0);
}

int find(char *dir, char *file) {
  int fd, n;
  struct dirent de;
  struct stat st;
  char buf[512];
  char* p;

  fd = open(dir, 0);
  if (fd < 0) {
    fprintf(2, "open %s failed\n", dir);
    return -1;
  }


  if (fstat(fd, &st) < 0) {
    fprintf(2, "stat %s failed\n", dir);
    return -1;
  }

  n = strlen(dir);

  switch (st.type) {
    case T_DIR:
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
          return 0;
        }

        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
          continue;
        }

        strcpy(buf, dir);
        p = buf + n;
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        stat(buf, &st);
        if (st.type == T_FILE) {
          if (strcmp(file, de.name) == 0) {
            printf("%s\n", buf);
          }
          continue;
        }
        find(buf, file);
      }
      break;

    // case T_FILE:
    // case T_DEVICE:
    default:
      return -1;
  }
  return 0;
}
