#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

void *sortFile(char *filename);

int main(int argc, char ** argv){

	pthread_t tid;
	int rc;

	rc = pthread_create(&tid, NULL, sortFile, NULL);

	if(rc){
		fprintf(stderr, "Thread error create\n");
		exit(1);
	}

}


void *sortFile(char *filename){
	
}