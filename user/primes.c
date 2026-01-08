#include "kernel/types.h"
// #include "kernel/stat.h"
#include "user/user.h"

int recurse_prime(void);

int main(int argc, char *argv[]) {
  if (argc != 1) {
    fprintf(2, "Usage: primes\n");
    exit(1);
  }
  int next_pipe_write = recurse_prime();

  for (int i = 2; i < 36; i++) {
    write(next_pipe_write, &i, 4);
  }
  close(next_pipe_write);
  wait(0);
  exit(0);
}

int recurse_prime(void) {
  int p[2];
  pipe(p);
  if (fork() == 0) {
    close(p[1]);
    int prime;
    read(p[0], &prime, 4);
    printf("prime %d\n", prime);
    int others;
    int next_pipe_write = -1;
    while (read(p[0], &others, 4) != 0) {
      if (others % prime != 0) {
        if (next_pipe_write == -1) {
          next_pipe_write = recurse_prime();
        }
        write(next_pipe_write, &others, 4);
      }
    }
    if (next_pipe_write != -1)
      close(next_pipe_write);
    wait(0); // if you remove this line it prints correctly except that the $
    // will be misaligned (still working try echo). this is because the $ prints
    // after the top-level parent exits. note that it only waits its child. if
    // you don't make each proc wait its child you will end up with children
    // printing primes after the top-level paren exits. this is problematic.
    exit(0);
  } else {
    close(p[0]);
    return p[1];
  }
}
