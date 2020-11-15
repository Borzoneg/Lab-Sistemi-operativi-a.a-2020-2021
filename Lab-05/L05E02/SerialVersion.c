#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define N 12

int main(int argc, char **argv){
	int v[N] = {12, 24, 6, 9, 18, 23, 34, 22, 1, 3, 7, 321};
	int i, j, fd, sx, dx, tmp, pid, statVal;


	if((fd = open("number.bin", O_RDWR)) == -1){ // opening of file and error control
		fprintf(stderr, "file error");
		exit(1);
	}

	for(i=0; i<N; i++) // writing of file
		write(fd, &v[i], sizeof(v[i]));	


	for(i=0; i<N; i++){
		for(j=0; j<N-i; j++){
			lseek(fd, sizeof(int) * j, SEEK_SET); // set for the current position in the file
			read(fd, &sx, sizeof(int)); // read the first value
			read(fd, &dx, sizeof(int)); // read the second value
			if(sx > dx){ // if they're inverted
				tmp = sx; // exchange them
				sx = dx;
				dx = tmp;
				lseek(fd, sizeof(int) * j, SEEK_SET); // reset into the right position
				write(fd, &sx, sizeof(int)); // write the values
				write(fd, &dx, sizeof(int));
				}
			}
		}

	for(i=0; i<N; i++){ // correct sorting control
			lseek(fd, sizeof(int)*i, SEEK_SET);
			read(fd, &tmp, sizeof(tmp));
			fprintf(stdout, "%d\n", tmp);
		}
	return 0;
	}
