#include "kernel/types.h"
#include "user/user.h"

int children(int n);

int redirect(int *ps);

int
main(int argc, char *argv[])
{
  int i;
  int ps[2];

  pipe(ps);

  if (fork() == 0) {
    // child
    redirect(ps);
    children(2);
  }

  close(ps[0]);

  for (i = 3; i < 36; i++) {
    write(ps[1], &i, sizeof(i));
  }

  close(ps[1]);

  wait(0);
  exit(0);
}

int
children(int n)
{
  printf("prime %d\n", n);
  int status, i;
  int hasChild;
  int ps[2];

  hasChild = 0;

  pipe(ps);

  for (;;) {
    status = read(0, &i, sizeof(i));

    if (status <= 0) {
      // pipe closed
      close(ps[1]);
      close(ps[0]);
      wait(0);
      exit(0);
    }

    if (i % n == 0) {
      // not prime, ignore
      continue;
    }

    write(ps[1], &i, sizeof(i));

    if (hasChild) {
      // hasChild, dont fork
      continue;
    }

    // no child exist
    // should only exec once
    if (fork() == 0) {
      redirect(ps);
      children(i);
    }
    hasChild = 1;
    close(ps[0]);
  }
}

int
redirect(int *ps)
{
  close(0);
  dup(ps[0]);
  close(ps[0]);
  close(ps[1]);

  return 0;
}
