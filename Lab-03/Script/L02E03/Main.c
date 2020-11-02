#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char ** argv){
	
	int pid1, pid2, statVal;

	fprintf(stdout, "P1\n");
	pid1 = fork();

	if(pid1 == 0){ // child P3 case
		fprintf(stdout, "P3\n");
		pid2 = fork();
		
		if (pid2 == 0) // child p6 case that have to print p6 and terminate to rejoin the P3 process
		{
			fprintf(stdout, "P6\n");
			sleep(5);
			exit(0);
		}
		else{ // the father P3 have to wait for P6 and became the process P8
			wait(&statVal);
			fprintf(stdout, "P8\n");
			sleep(5);
			exit(0); // terminate the process P8 to rejoin P7, forming P9
		}
	}

	else{ // father that become P2 case
		fprintf(stdout, "P2\n");
		pid2 = fork();
		if (pid2 == 0) // child P5 case that have to print p6 and terminate to rejoin the P3 process
		{
			fprintf(stdout, "P5\n");
			sleep(5);
			exit(0);
		}
		else{ // the father became P4
			fprintf(stdout, "P4\n");
			wait(&statVal); // wait for process P5
			fprintf(stdout, "P7\n"); // became process P7
		}

	}	
	wait(&statVal); // wait for process P8 
	fprintf(stdout, "P9\n"); // final process
	return(0);

}