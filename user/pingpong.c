#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int p1[2];
	pipe(p1);
	char buf[20];
	int pid = fork();

	if(pid==0){
		read(p1[0], buf, 1);
		close(p1[0]);
		printf("%d: received ping\n", getpid());
		write(p1[1], "@", 1);
		close(p1[1]);
		exit(0);
	}
	
	else if(pid>0){
		write(p1[1], "@", 1);
		close(p1[1]);
		read(p1[0], buf, 1);
		close(p1[0]);
		printf("%d: received pong\n", getpid());
		exit(0);
	}
	
	return 0;
}
