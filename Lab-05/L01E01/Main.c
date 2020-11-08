#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

// to get the input from a file instead of the stdin
// ./Main < infile.txt  

// to redirect the stdout to evenfile and the stderr to oddfile
//./Main 1> oddFile.txt 2> evenFile.txt 

int main(int argc, char ** argv){
	int input = -1 ;

	while(1){
		scanf("%d", &input);	
		if(input == 0)
			return(0);
		if(input % 2 != 0) // odd case
			fprintf(stderr, "%d\n", input);
		else
			fprintf(stdout, "%d\n", input);
	}
	return(0);
}