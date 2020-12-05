#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	if(!fork()){
		fprintf(stdout, "%d\n", getpid());
		return 0;
	}
	while(1);
	return 0;
}