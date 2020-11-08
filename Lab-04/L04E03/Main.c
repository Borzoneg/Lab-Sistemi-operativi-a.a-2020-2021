#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#define OPTIONS 10
#define LEN 20
#define MAX 100

int main(int argc, char ** argv){
	if(argc != 2){
		fprintf(stderr, "%s usage: %s file with commands name\n", argv[0], argv[0]);
		exit(1);
	}

	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL){
		fprintf(stderr, "Problem with the file\n");
	}
	char **mat, buf[LEN], command[MAX];
	int i, j, pid;

	mat = malloc(sizeof(char*) * OPTIONS);				
	
	/*** System function mode ***/
	
	// while(fscanf(fp, "%s", buf) != EOF){ // read the file word by word
	// 	if(strcmp(buf, "end")){ // if different from the end of line
	// 		strcat(command, buf); // add to the command
	// 		strcat(command, " "); // add the space between parameters
	// 	}
	// 	else{ // if we reached the end of the line we have to execute the command
	// 		fprintf(stdout, "Executing via System: %s\n", command);
	// 		system(command); // executing the command
	// 		command[0] = '\0'; // clean the string
	// 	}

	// }

	/*** Exec mode ***/
	
	i = 0;
	while(fscanf(fp, "%s", buf) != EOF){ // read the file word by word
		if(strcmp(buf, "end")){ // if different from the end of line
			mat[i] = strdup(buf); // collect in the mat the parameter

			i++; // increment i
		}
		else{ // if we reached the end of the line we have to execute the command
			mat[i] =  NULL; // to tell the exec that the parameters of the funct are ended
			fprintf(stdout, "Executing via exec: %s parameters:\n", mat[0]);
			for(j=0; j<i; j++)
				fprintf(stdout, "%s\n", mat[j]);
			pid = fork();
			if(!pid){ // the son
				execvp(mat[0], mat); // make an exec and die
			}
			else // the father
				sleep(3); // just sleep
			i = 0; // start using the mat from the beginnning
			mat[i] = malloc(sizeof(char) * LEN);
		}

	}	
}