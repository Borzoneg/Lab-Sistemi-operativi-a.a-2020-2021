#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX 20

void manager(int SIG){
	if(SIG == SIGUSR1)
		return;
}

int main(int argc, char **argv){
	int pid, pipe1[2], pipe2[2];
	char ch = 'x';

	signal(SIGUSR1, manager); // instance signal manager

	if(pipe(pipe1) != 0){ // error control on first pipe opening
		fprintf(stderr, "Pipe creation failed");
		exit(1);
	}

	if(pipe(pipe2) != 0){ // error control on 2nd pipe opening
		fprintf(stderr, "Pipe creation failed");
		exit(1);
	}

	pid = fork();
	if(pid < 0){ // error control on first fork
		fprintf(stderr, "First fork failed\n");
		exit(1);	
	}

	if(pid){ // father case
		while(1){
			close(pipe1[0]);
			close(pipe2[1]);
			fprintf(stdout, "I'm the father PID: %d\n", getpid());
			write(pipe1[1], &ch, sizeof(char)); // write char on pipe1
			read(pipe2[0], &ch, sizeof(char)); // read (as a wait) on pipe 2
		}
	}
	else{ // son case	
		while(1){
			close(pipe1[1]);
			close(pipe2[0]);
			read(pipe1[0], &ch, sizeof(char)); // read on pipe 1 as a wait
			fprintf(stdout, "I'm the son PID: %d\n", getpid());
			write(pipe2[1], &ch, sizeof(char)); // write char on pipe 2 to unlock the father
		}
 	}
}
