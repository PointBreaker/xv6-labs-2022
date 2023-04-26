#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int 
main(int argc, char * argv[]) {
  char *arg_array[8][MAXARG];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < MAXARG; j++) {
      arg_array[i][j] = (char *)malloc(8 * sizeof(char));
    }
  }
  char cmd[64];
  if (argc < 2) {
    fprintf(2, "usage: xargs <command> ..\n");
    exit(1);
  }
  strcpy(cmd, argv[1]);
  for (int i = 0; i < 8; i++) {
    for (int j = 1; j < argc; j++) {
      strcpy(arg_array[i][j-1], argv[j]);
    }
  }
  char c = 1;
  int cnt = 0;
  int num_arg = 0;
  int cur_arg = argc - 1;
  while (read(0, &c, 1) != 0) {
    if (c != '\n') {
      arg_array[num_arg][cur_arg][cnt++] = c;
    } else {
      arg_array[num_arg][cur_arg+1] = 0;
      num_arg++;
      cnt = 0;
    }
  }

  // verbose
  // printf("==verbose==\n");
  // printf(cmd);
  // printf("\n");
  // for (int i = 0; i < num_arg; i++) {
  //   for (int j = 0; j < argc; j++) {
  //     printf(arg_array[i][j]);
  //     printf(" ");
  //   }
  //   printf("\n");
  // }
  // printf("========\n");

  for (int i = 0; i < num_arg; i++) {
    int pid = fork();
    int status;
    if (pid == 0) {
      exec(cmd, arg_array[i]);
    } else {
      wait(&status);
    }
  }
  exit(0);
}