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

	int n, i, rc;
	requiredInfo *parameters;
	void *exit_status;

	if(argc != 4){
		fprintf(stderr, "Not enough parameters");
		exit(1);
	}

	n = atoi(argv[1]);

	
	pthread_t *tid = malloc(sizeof(pthread_t)*n);

	for(i=0; i<n; i++){
		parameters = malloc(sizeof(requiredInfo));
		parameters->i = i+1;
		parameters->in = strdup(argv[2]);
		parameters->out = strdup(argv[3]);

		rc = pthread_create(&tid[i], NULL, sortFile, (void*) parameters);
		if(rc){
			fprintf(stderr, "Thread creation issue");
			exit(0);
		}
	}

	for(i=0; i<n; i++){
		pthread_join(tid[i], &exit_status);
	}

	return (0);

}


void *sortFile(void *data){

	requiredInfo *dd = (requiredInfo*) data;
	
	char * file_in = malloc(sizeof(char)*N);
	char * file_out = malloc(sizeof(char)*N);

	sprintf(file_in, "%s%d", dd->in, dd->i);
	sprintf(file_out, "%s%d", dd->out, dd->i);

	FILE *f_in = fopen(file_in, "r");
	FILE *f_out = fopen(file_out, "w");


	fprintf(stdout, "file_in: %s file_out:%s i: %d\n", file_in, file_out, dd->i);

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
	
	fclose(f_in);
	fclose(f_out);

	free(file_in);
	free(file_out);

	free(dd->in);
	free(dd->out);
	free(dd);

	i -=n;
	return(NULL);
}