#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

#define MAX 100

void *reader(void *filename);
void *inverter();	
void *writer(void *filename);

/* semaphores declaration */
sem_t *S1;
sem_t *S2;
sem_t *S3;
sem_t *S4;

/* global variable */
char line[MAX], inverted_line[MAX];
pthread_t t_reader, t_writer, t_inverter;
int written_line=0, read_line=0;

int main(int argc, char **argv){
	
	if(argc != 3){
		fprintf(stderr, "Not enough parameters");
		exit(1);
	}
	
	/* semaphores init */
	S1 = malloc(sizeof(sem_t));
	S2 = malloc(sizeof(sem_t));
	S3 = malloc(sizeof(sem_t));
	S4 = malloc(sizeof(sem_t));
	sem_init(S1, 0, 1);
	sem_init(S2, 0, 1);
	sem_init(S3, 0, 0);
	sem_init(S4, 0, 4);

	
	void *exitStatus;

	/* threads creation */
	pthread_create(&t_reader, NULL, reader, ((void*) argv[1]));
	pthread_create(&t_inverter, NULL, inverter, NULL);
	pthread_create(&t_writer, NULL, writer, ((void*) argv[2]));

	/* threads join */
	pthread_join(t_reader, &exitStatus);
	pthread_join(t_writer, &exitStatus);
	pthread_join(t_inverter, &exitStatus);

	/* semaphore destruction */
	sem_destroy(S1);	
	sem_destroy(S2);
	sem_destroy(S3);

	return 0;
}


void *reader(void *filename){
	char *fn = (char*)filename;
	FILE *fp = fopen(fn, "r");
	if(fp == NULL){
		fprintf(stderr, "Erorr with file\n");
		exit(1);
	}
	while(1){
		sem_wait(S1);
		
		if(fgets(line, MAX, fp) == NULL){ // once we fond out that the file it's finished we can close the threads
			while(written_line!=read_line); // wait for the writer to write the last line then terminate
			fclose(fp);
			pthread_cancel(t_inverter);
			pthread_cancel(t_writer);
			return(NULL);
		}
		read_line++; // keep track of how many lines we read
		sem_post(S2);
	}
}


void *inverter(){
	char tmp;
	int i;
	while(1){
		sem_wait(S2);
		sem_wait(S2);
		
		strcpy(inverted_line, line); // copy string we have to manipulate
		sem_post(S1); // so the reader can resume his work
		
		/* swap of last and first char */
		tmp = inverted_line[0];
		inverted_line[0] = inverted_line[strlen(inverted_line)-2];
		inverted_line[strlen(inverted_line)-2] = tmp; 
		
		for(i=0; i<strlen(inverted_line); i++) // for each letter in the string
			inverted_line[i] = islower(inverted_line[i]) ? inverted_line[i] - 32 : inverted_line[i]; // if a lower letter capitalize it			

		sem_post(S3);
	}
	return(NULL);
}


void *writer(void *filename){
	char *fn = (char*)filename;
	FILE *fp = fopen(fn, "w");
	
	if(fp == NULL){
		fprintf(stderr, "Erorr with file\n");
		exit(1);
	}
	
	while(1){
		sem_wait(S3);
		
		fprintf(fp, "%s", inverted_line);
		written_line++; // keep track of how many lines we wrote

		sem_post(S2);
	}
	return(NULL);
}