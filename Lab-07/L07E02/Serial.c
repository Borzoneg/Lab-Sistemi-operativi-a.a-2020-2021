#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#define N 30

typedef struct requiredInfo
{
	char * in;
	char *out;
	int i;
}requiredInfo;

void *sortFile(void *data);

int main(int argc, char ** argv){

	
	int n, i;
	requiredInfo *parameters;

	if(argc != 4){
		fprintf(stderr, "Not enough parameters");
		exit(1);
	}

	n = atoi(argv[1]);

	parameters = malloc(sizeof(requiredInfo)*n);

	for(i=0; i<n; i++){
		fprintf(stdout, "%d\n", n);
		parameters[i].i = i+1;
		parameters[i].in = strdup(argv[2]);
		parameters[i].out = strdup(argv[3]);
		sortFile((void*) &parameters[i]);
	}
	return (0);
}


void *sortFile(void *data){

	requiredInfo *dd = (requiredInfo*) data;

	char * file_in = malloc(sizeof(char)*N);
	char * file_out = malloc(sizeof(char)*N);

	sprintf(file_in, "%s%d", dd->in, dd->i);
	sprintf(file_out, "%s%d", dd->out, dd->i);

	fprintf(stdout, "file_in: %s\n", file_in);
	fprintf(stdout, "file_out: %s\n", file_out);

	FILE *f_in = fopen(file_in, "r");
	FILE *f_out = fopen(file_out, "w");

	int n, i, j, *v, tmp;
	fscanf(f_in, "%d", &n);

	v = malloc(sizeof(int)*n);

	for(i=0; i<n; i++)
		fscanf(f_in, "%d", &v[i]);

	for(i=0; i<n; i++){
		for(j=0; j<n-i-1; j++){
			if(v[j] > v[j+1]){
				tmp = v[j];
				v[j] = v[j+1];
				v[j+1] = tmp;
			}
		}
	}

	for(i=0; i<n; i++)
		fprintf(f_out, "%d\n", v[i]);
	
	//pthread_exit(0);
}