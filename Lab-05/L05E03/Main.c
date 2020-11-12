#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

void manager(int SIG){
	if(SIG == SIGALRM){
		fprintf(stdout, "Received ALRM\n");
		return;
	}
}

int main(int argc, char **argv){

	int pid;
	signal(SIGALRM, manager); // instance signal manager for the SIGALRM signal

	pid = fork();
	if(pid){ // father case
		fprintf(stdout, "Father: %d\tChild:%d\n", getpid(), pid);
		while(1){
			fprintf(stdout, "Father woke up\n");
			sleep(1); // sleep 1 second before sending the kill to avoid race conditions problems
			kill(pid, SIGALRM); //send the signal to the son
			pause(); // set himself to wait for a signal
		}
	}
	else{ // son case
		while(1){
			pause(); // set himself to wait for a signal
			fprintf(stdout, "Child woke up\n");
			sleep(1); // sleep 1 second before sending the kill to avoid race conditions problems
			kill(getppid(), SIGALRM); // send the signal to his father
			}
	}
	return 0;
}