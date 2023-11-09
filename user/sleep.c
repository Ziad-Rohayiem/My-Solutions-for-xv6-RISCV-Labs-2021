#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Error Missing Argument\n");
	}
	else{
		int x = atoi(argv[1]);
		sleep(x);
	}
	
	exit(0);
}
