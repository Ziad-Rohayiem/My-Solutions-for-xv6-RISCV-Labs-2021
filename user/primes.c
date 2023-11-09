#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void rec(int fd)
{
    int num;
    read(fd, &num, sizeof(int));
    printf("prime %d\n", num);
    
    int p[2];
    pipe(p);
    int tmp = -1;
    while (1) {
        int n = read(fd, &tmp, sizeof(int));
        if (n <= 0) {
            break;
        }
        if (tmp % num != 0) {
            write(p[1], &tmp, sizeof(int));
        }
    }
    if (tmp == -1) {
        close(p[1]);
        close(p[0]);
        close(fd);
        return;
    }
    int pid = fork();
    if (pid == 0){
        close(p[1]);
        close(fd);
        rec(p[0]);
        close(p[0]);
    }
    else {
        close(p[1]);
        close(p[0]);
        close(fd);
        wait(0);
    }
}

int main(int *argc, char **argv[])
{
    int p[2];
    pipe(p);
    for (int i = 2; i < 35; i++) {
        //int n = i;
        write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    rec(p[0]);
    close(p[0]);

    exit(1);
}
