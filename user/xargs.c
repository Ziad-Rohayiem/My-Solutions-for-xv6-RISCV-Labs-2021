#include "kernel/types.h"
// #include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void xargs(char *argv[], int new_argc) {
  int started_arg = 0;
  int read_eof = 0;
  char buf[512];
  memset(buf, 0, 512);
  // read until you hit a newline or a EOF and copy that buf ontoo the end ofo
  // argv;
  while (1) {
    int n = read(0, buf + started_arg, 1);
    started_arg += 1;
    if (n == 0 || buf[started_arg - 1] == '\n') {
      if (n == 0 && read_eof > 0)
        break;
      else
        read_eof += 1;

      if (buf[started_arg - 1] == '\n' && started_arg == 1) {
        started_arg--;
        continue;
      }
      if (buf[started_arg - 1] == '\n')
        buf[started_arg - 1] = 0;
      if (fork() == 0) {
        argv[new_argc] = buf;
        // printf("argc: %d\n", new_argc + 1);
        // for (int i = 0; i < new_argc + 1; i++) {
        //   printf("argv[%d]=%s\n", i, argv[i]);
        // }
        exec(argv[0], argv);
      } else {
        wait(0);
      }
      memset(buf, 0, started_arg);
      started_arg = 0;
    }
  }
}

int main(int argc, char *argv[]) {
  // reads lines from standard input
  // rest of argv is the exec command
  if (argc >= MAXARG) {
    fprintf(2, "Error: number of arguments have to be less than %d", MAXARG);
    exit(1);
  }
  // TODO: make echo as default command
  char *argv_new_cmd[MAXARG];
  memmove(argv_new_cmd, argv + 1, (argc - 1) * sizeof(char *));
  xargs(argv_new_cmd, argc - 1);
  exit(0);
}
