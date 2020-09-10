#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFSIZE 512

int main(int argc, char *argv[]) {
  int i;
  char buf[BUFSIZE];
  char* child_argv[MAXARG];

  while (*gets(buf, BUFSIZE)) {
    // get rid of '\n'
    i = strlen(buf);
    buf[i - 1] = 0;

    if (fork() == 0) {
      // child
      for (i = 0; i < argc - 1; i++) {
        child_argv[i] = argv[i + 1];
      }

      child_argv[i] = buf;

      exec(child_argv[0], child_argv);
    }

    wait(0);
  }

  exit(0);
}
