#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int n, int * p_old) {
  int buf;
  int status;
  int p[2];
  if (pipe(p) < 0) {
    fprintf(2, "pipe failed\n");
    exit(1);
  }
  printf("prime %d\n", n);
  int cpid = fork();
  if (cpid < 0) {
    fprintf(2, "fork failed\n");
    exit(1);
  }
  if (n == 2) { // init
    if (cpid == 0) { // child process
      close(p[1]);
      if (read(p[0], &buf, 4) != 0) {
          primes(buf, p);
      }
      close(p[0]);
      exit(0);
    } else { // parent process
      close(p[0]);
      for (int i = n + 1; i <= 35; i++) {
        if (i % n != 0) {
          write(p[1], &i, 4);
        }
      }
      close(p[1]);
      wait(&status);
      exit(0);
    }
  } else {
    if (cpid == 0) { // child process
      close(p_old[0]);
      close(p[1]);
      if (read(p[0], &buf, 4) != 0) {
        primes(buf, p);
      }
      close(p[0]);
      exit(0);
    } else { // parent process
      close(p[0]);
      while (read(p_old[0], &buf, 4) != 0) {
        if (buf % n != 0) {
          write(p[1], &buf, 4);
        }
      }
      close(p_old[0]);
      close(p[1]);
      wait(&status);
      exit(0);
    }
  }
}

int
main(int argc, char *argv[])
{
  primes(2, 0);
  exit(0);
}