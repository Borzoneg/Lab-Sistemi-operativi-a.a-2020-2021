#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int *s_init(int start_value);
int s_signal(int f_pipe[2]);
char s_wait(int f_pipe[2]);
int s_destroy(int f_pipe[2]);
int routine(char id);
int process_a();
int process_b();
int process_c();
int process_d();
int process_e();
int process_f();
int process_g();
int process_h();
int process_i();

int *S1;
int *S2;
int *S3;
int *S4;
int *S5;
int *S6;
int *S7;
int *S8;

int main(int argc, char **argv)
{
	// init of semaphores
	S1 = s_init(1);
 	S2 = s_init(0);
 	S3 = s_init(0);
 	S4 = s_init(0);
 	S5 = s_init(0);
 	S6 = s_init(0);
 	S7 = s_init(0);
 	S8 = s_init(0);

 	while(1){
	if(!fork()) // 'A' process
		return(process_a());
	if(!fork()) // 'B' process
		return(process_b());
	if(!fork()) // 'C' process
		return(process_c());
	if(!fork()) // 'D' process
		return(process_d());
	if(!fork()) // 'E' process
		return(process_e());
	if(!fork()) // 'F' process
		return(process_f());
	if(!fork()) // 'G' process
		return(process_g());
	if(!fork()) // 'H' process
		return(process_h());
	if(!fork()) // 'I' process
		return(process_i());
	}
		
	

	return 0;
}

int *s_init(int start_value){
	int i;
	char ch = 'x';
	int *f_pipe = malloc(sizeof(int)*2);
	if(pipe(f_pipe)){
		fprintf(stderr, "Error!\n");
		exit(1);
	}
	for(i=0; i<start_value; i++){
		if(write(f_pipe[1], &ch, sizeof(char)) != 1){
			fprintf(stderr, "Error!\n");
			exit(1);
		}
	}
	return f_pipe;
}

int s_signal(int f_pipe[2]){
	char ch = 'x';
	if(write(f_pipe[1], &ch, sizeof(char)) != 1){
		fprintf(stderr, "Error!\n");
		exit(1);
	}
	return (0);
}


char s_wait(int f_pipe[2]){
	char ch;
	if(read(f_pipe[0], &ch, sizeof(char)) != 1){
		fprintf(stderr, "Error!\n");
		exit(1);
	}
	return (ch);	
}


int s_destroy(int f_pipe[2]){
	close(f_pipe[0]);
	close(f_pipe[1]);
	return(0);
}


int process_a(){
	s_wait(S1);
	fprintf(stdout, "A\n");
	s_signal(S2);
	s_signal(S3);
	s_signal(S4);	
	return(0);
}


int process_b(){
	s_wait(S2);
	fprintf(stdout, "B\n");
	s_signal(S8);
	return(0);
}


int process_c(){
	s_wait(S3);
	fprintf(stdout, "C\n");
	s_signal(S5);
	s_signal(S5);
	return(0);
}


int process_d(){
	s_wait(S4);
	fprintf(stdout, "D\n");
	s_signal(S7);	
	return(0);
}


int process_e(){
	s_wait(S5);
	fprintf(stdout, "E\n");
	s_signal(S6);	
	return(0);
}


int process_f(){
	s_wait(S5);
	fprintf(stdout, "F\n");
	s_signal(S6);
	return(0);
}
		

int process_g(){
	s_wait(S6);
	s_wait(S6);
	fprintf(stdout, "G\n");
	s_signal(S8);
	return(0);
}


int process_h(){
	s_wait(S7);
	fprintf(stdout, "H\n");
	s_signal(S8);	
	return(0);
}


int process_i(){
	s_wait(S8);
	s_wait(S8);
	s_wait(S8);
	fprintf(stdout, "I\n");
	s_signal(S1);	
	return(0);
}