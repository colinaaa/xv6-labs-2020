#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2 || argc > 2){
    fprintf(2, "Usage: sleep time\n");
    exit(0);
  }

  // convert ascii to int
  i = atoi(argv[1]);

  // do syscall
  sleep(i);

  exit(0);
}
