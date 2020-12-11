#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_THREAD 9

void *process_a();
void *process_b();
void *process_c();
void *process_d();
void *process_e();
void *process_f();
void *process_g();
void *process_h();
void *process_i();

sem_t *S1;
sem_t *S2;
sem_t *S3;
sem_t *S4;
sem_t *S5;
sem_t *S6;
sem_t *S7;
sem_t *S8;
sem_t *S9;
sem_t *S10;
sem_t *S11;
sem_t *S12;

int main(int argc, char **argv){
	S1 = malloc(sizeof(sem_t));
	S2 = malloc(sizeof(sem_t));
	S3 = malloc(sizeof(sem_t));
	S4 = malloc(sizeof(sem_t));
	S5 = malloc(sizeof(sem_t));
	S6 = malloc(sizeof(sem_t));
	S7 = malloc(sizeof(sem_t));
	S8 = malloc(sizeof(sem_t));
	S9 = malloc(sizeof(sem_t));
	S10 = malloc(sizeof(sem_t));
	S11 = malloc(sizeof(sem_t));
	S12 = malloc(sizeof(sem_t));
	
	// init of semaphores
	sem_init(S1, 0, 1);
 	sem_init(S2, 0, 0);
 	sem_init(S3, 0, 0);
 	sem_init(S4, 0, 0);
 	sem_init(S5, 0, 0);
 	sem_init(S6, 0, 0);
 	sem_init(S7, 0, 0);
 	sem_init(S8, 0, 0);
 	sem_init(S9, 0, 0);
 	sem_init(S10, 0, 0);
 	sem_init(S11, 0, 0);
 	sem_init(S12, 0, 0);

 	pthread_t t[NUM_THREAD];
 	int rc;

 	
	rc = pthread_create(&t[0], NULL, process_a, NULL); // create thread for process A
	if(rc) exit(1);

	rc = pthread_create(&t[1], NULL, process_b, NULL); 
	if(rc) exit(1);

	rc = pthread_create(&t[2], NULL, process_c, NULL);
	if(rc) exit(1);
	
	rc = pthread_create(&t[3], NULL, process_d, NULL);
	if(rc) exit(1);
	
	rc = pthread_create(&t[4], NULL, process_e, NULL);
	if(rc) exit(1);
	
	rc = pthread_create(&t[5], NULL, process_f, NULL);
		if(rc) exit(1);
	
	rc = pthread_create(&t[6], NULL, process_g, NULL);
	if(rc) exit(1);
	
	rc = pthread_create(&t[7], NULL, process_h, NULL);
	if(rc) exit(1);
	
	rc = pthread_create(&t[8], NULL, process_i, NULL);
	if(rc) exit(1);
	
	void *returnValue;
	int i;
	for(i=0; i<NUM_THREAD; i++){
		pthread_join(t[i], &returnValue);
	}
	return 0;
}


void *process_a(){
	while(1){
		sem_wait(S1);
		fprintf(stdout, "A\n");
		sem_post(S2);
		sem_post(S3);
		sem_post(S4);
	}
	return(NULL);
}


void *process_b(){
	while(1){
		sem_wait(S2);
		fprintf(stdout, "B\n");
		sem_post(S10);
	}
	return(NULL);
}


void *process_c(){
	while(1){
		sem_wait(S3);
		fprintf(stdout, "C\n");
		sem_post(S5);
		sem_post(S6);
	}
	return(NULL);
}


void *process_d(){
	while(1){
		sem_wait(S4);
		fprintf(stdout, "D\n");
		sem_post(S9);
	}
	return(NULL);
}


void *process_e(){
	while(1){
		sem_wait(S5);
		fprintf(stdout, "E\n");
		sem_post(S7);
	}
	return(NULL);
}


void *process_f(){
	while(1){
		sem_wait(S6);
		fprintf(stdout, "F\n");
		sem_post(S8);
	}
	return(NULL);
}
		

void *process_g(){
	while(1){
		sem_wait(S7);
		sem_wait(S8);
		fprintf(stdout, "G\n");
		sem_post(S11);
	}
	return(NULL);
}


void *process_h(){
	while(1){
		sem_wait(S9);
		fprintf(stdout, "H\n");
		sem_post(S12);
	}
	return(NULL);
}


void *process_i(){
	while(1){
		sem_wait(S10);
		sem_wait(S11);
		sem_wait(S12);
		fprintf(stdout, "I\n");
		sem_post(S1);
	}
	return(NULL);
}
