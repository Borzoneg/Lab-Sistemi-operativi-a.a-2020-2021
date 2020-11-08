#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>


int main(int argc, char **argv){

	if (argc != 3){
		fprintf(stderr, "%s usage: %s n t\n\n", argv[0], argv[0]);
		exit(1);
	}

	int n = atoi(argv[1]), t = atoi(argv[2]);
	
	int i, pid1, pid2, fatherPid;

	for(i=0; i<n; i++){ // Each iteration will create 2^i process, so we have to iterate n times
		pid1 = fork(); // first fork
		if(pid1 == 0){ // child save the pid of his father for future needings
			fatherPid = getppid();
			//fprintf(stdout, "Child1: %d ; Father: %d\n", getpid(), getppid());	
		}
		else{ // father
			pid2 = fork(); // 2nd fork
			if(pid2 == 0){ // child save the pid of his father for future needings
				fatherPid = getppid();
				//fprintf(stdout, "Child2: %d ; Father: %d\n", getpid(), getppid());	
			}
			else{ // the father have terminate
				//fprintf(stdout, "Closing process: %d\n", getpid());		
				sleep(1);
				exit(0);
			}
		}
	}
	sleep(t); // the process that didn't terminate inside the cycle are leaf process, they have to wait t and print a message
	fprintf(stdout, "Leaf process: %d ; Father: %d\n", getpid(), fatherPid);
	exit(0);
}
