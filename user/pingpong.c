#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  char buf[] = "hello";
  int pid;
  int cpid = fork();
  if (cpid == 0) { // child
    pid = getpid();
    read(p[0], buf, 1);
    printf("%d: received ping\n", pid);
    write(p[1], buf, 1);
    close(p[0]);
    close(p[1]);
  } else { // parent
    pid = getpid();
    write(p[1], buf, 1);
    read(p[0], buf, 1);
    printf("%d: received pong\n", pid);
    close(p[0]);
    close(p[1]);
  }
  exit(0);
}