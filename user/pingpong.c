#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  char buf;
  int ps[2];
  int pps[2];

  if (argc != 1) {
    fprintf(2, "Usage: pingpong\n");
    exit(0);
  }
  pipe(ps);
  pipe(pps);

  if (fork() == 0) {
    // child
    pid = getpid();

    read(ps[0], &buf, 1);
    printf("%d: received ping\n", pid);

    write(pps[1], &buf, 1);
    exit(0);
  }

  // parent
  pid = getpid();
  write(ps[1], "a", 1);
  // close(ps[1]);

  read(pps[0], &buf, 1);
  printf("%d: received pong\n", pid);

  exit(0);
}
