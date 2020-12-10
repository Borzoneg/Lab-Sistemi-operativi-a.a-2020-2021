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
	int i;
}requiredInfo;

void *sortFile(void *data);
int merge (int *v1,int *v2,int *vmerge,int size_v1, int size_v2);

int main(int argc, char ** argv){

	int n, i, j, rc, lenght;
	requiredInfo *parameters;
	void *exit_status;
	char tmp[30];

	if(argc != 4){
		fprintf(stderr, "Not enough parameters");
		exit(1);
	}

	n = atoi(argv[1]);

	sprintf(tmp, "%s1", argv[2]);
	FILE *fp = fopen(tmp, "r"); // read the first file to get the number of int in each file
	fscanf(fp, "%d", &lenght); // save the number of digit in each file
	fclose(fp);

	int **matrix = malloc(sizeof(int*)*n); // create a matrix, each line for each thread
	for(i=0; i<n; i++)
		matrix[i] = malloc(sizeof(int)*lenght); // each line contains the number of digits that will have to store from the file

	pthread_t *tid = malloc(sizeof(pthread_t)*n); // create the tids vector

	for(i=0; i<n; i++){
		parameters = malloc(sizeof(requiredInfo)); // create and allocate the parameters for the thread
		parameters->i = i+1; 
		parameters->in = strdup(argv[2]);

		rc = pthread_create(&tid[i], NULL, sortFile, (void*) parameters); // start the thread
		if(rc){ // errore management
			fprintf(stderr, "Thread creation issue");
			exit(0);
		}
	}

	int *final_v = malloc(sizeof(int)*n*lenght); // create a vector for all the values in the files
	int *tmp_v = malloc(sizeof(int)*n*lenght); // create a copy for the merge function
	int current_lenght = 0;
	for(i=0; i<n; i++){ // for each thread we created
		pthread_join(tid[i], &exit_status); // get the result in exit_status
		matrix[i] = exit_status;
		current_lenght = merge(tmp_v, matrix[i], final_v, current_lenght, lenght);
		for(j=0; j<current_lenght; j++)
			tmp_v[j] = final_v[j];
		// for(int j=0; j<current_lenght;printf("%d\n", final_v[j++]));
		// printf("\n");
	}
	for(i=0; i<(n*lenght); i++){
		printf("%d\n", final_v[i]);
	} 
	
	return (0);

}


void *sortFile(void *data){

	requiredInfo *dd = (requiredInfo*) data; // convert the parameters
	
	char * file_in = malloc(sizeof(char)*N); // allocate the string

	sprintf(file_in, "%s%d", dd->in, dd->i); // generate the string for the input file
	
	FILE *f_in = fopen(file_in, "r"); 
	
	int n, i, j, *v, tmp;
	fscanf(f_in, "%d", &n);

	v = malloc(sizeof(int)*n);
	
	for(i=0; i<n; i++) // scan of the file
		fscanf(f_in, "%d", &v[i]);

	for(i=0; i<n; i++){ // bubblesort
		for(j=0; j<n-i-1; j++){
			if(v[j] > v[j+1]){
				tmp = v[j];
				v[j] = v[j+1];
				v[j+1] = tmp;
			}
		}
	}
	
	// clean of data structures, close of files
	fclose(f_in);

	free(file_in);

	free(dd->in);
	free(dd);

	return((void*) v);
}



int merge (int *v1,int *v2,int *vmerge,int size_v1, int size_v2){ 
  int size_vmerge;
  int i,j,k;
 
  i=j=k=0; 
  while (i<size_v1 && j<size_v2) { 
    if (v1[i]<v2[j])
		vmerge[k++]=v1[i++];
    else
		vmerge[k++]=v2[j++];
   } 
   while (i<size_v1) {   
	vmerge[k++]=v1[i++];
   }
   while (j<size_v2) {
     vmerge[k++]=v2[j++];
   }
   size_vmerge=size_v1+size_v2; 
   return size_vmerge;  
}