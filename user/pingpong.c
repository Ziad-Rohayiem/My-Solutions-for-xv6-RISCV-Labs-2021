#include "kernel/types.h"
// #include "kernel/stat.h"
#include "user/user.h"

// There are two main concerns I discovered with my solution here. The first is that I am using one pipe for both parent and child
// if not handled properly. better practice is to use p1 (parent -> child) so parent only writes for p1 and p2 (child -> parent)
// and carefully close each fd once it is not used. The second comment is that I am not really handling any errors from pipe,
// fork, read, or write. These errors have to be handled carefully for correct programs in a real OS.

int main(int argc, char *argv[]) {
  if (argc != 1) {
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }
  int p[2];
  pipe(p);
  char buf[1];
  if (fork() == 0) {            // child
    read(p[0], buf, 1); // read byte
    printf("%d: received ping\n", getpid());
    write(p[1], buf, 1);                 // write byte second time
  } else {                               // parent
    write(p[1], "b" /*random byte*/, 1); // write byte first time
    read(p[0], buf, 1);          // read byte second time
    printf("%d: received pong\n", getpid());
  }
  exit(0);
}
