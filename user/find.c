#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define EQUAL 0

int
main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "usage: find <path> <pattern>\n");
    exit(1);
  }
  exit(0);
}