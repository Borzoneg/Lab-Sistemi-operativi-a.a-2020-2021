#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX 20

void manager(int SIG){
	if(SIG == SIGUSR1)
		return;
}

int main(int argc, char **argv){
	int pid, pid2, file_extr[2], pipe_w, pipe_r, n;
	char ch, *buf = malloc(sizeof(char)*MAX);

	signal(SIGUSR1, manager); // instance signal manager

	if(pipe(file_extr) != 0){ // error control on pipe opening
		fprintf(stderr, "Pipe creation failed");
		exit(1);
	}

	pipe_r = file_extr[0];
	pipe_w = file_extr[1];

	pid = fork();
	if(pid < 0){ // error control on first fork
		fprintf(stderr, "First fork failed\n");
		exit(1);	
	}
	if(pid){ // first fork father case
		pid2 = fork();
		if(pid2 < 0){ // error control on 2nd fork
			fprintf(stderr, "Second fork failed\n");
			exit(1);	
		}
		if(pid2){ // second fork father case
			close(pipe_r); // I won't read anything from the pipe with the father so i can close it
			write(pipe_w, &pid2, sizeof(pid2)); // write the pid2 on the pipe to give it to 1st son
			sleep(3); // sleep 1 for race conditions problems with first son
			close(pipe_w); // close the pipe for the father
			kill(pid, SIGUSR1); // wake up the first son
			pause();			
		}

		else{ // 2nd son case
			close(pipe_w); // close one part of the pipe
			fprintf(stdout, "My [Son2] Brother pid: %d\n", pid);
			while(1){
				pause(); // wait for the signal of read done form stdout
				n = 1;
				while((n = read(pipe_r, &ch, sizeof(ch))) == 1){ // read char by char from pipe
					if(ch == '\0')
						break;
					ch -= 32; // convert into capitalized letter
					setbuf(stdout, NULL);
					fprintf(stdout, "%c", ch); // print char by char on stdout
				}
				fprintf(stdout, "\n"); // when we finish a word we print a LF
				sleep(3);
				kill(pid, SIGUSR1); //send the signal to the brother that confirm read of the data
			}	
		}
	}
	else{
		pause(); // wait for the father to write the value of the pid 2 on the pipe
		read(pipe_r, &pid2, sizeof(pid2)); // read the value of pid2 from the father
		close(pipe_r); // close one part of the pipe
		fprintf(stdout, "My [Son1] Brother pid: %d\n", pid2);
		while(1){
			fprintf(stdout, "Insert lower case word\n");
			fscanf(stdin, "%s", buf);
			write(pipe_w, buf, sizeof(char) * strlen(buf)); // write char on pipe
			ch = '\0'; // define the end of the word written
			write(pipe_w, &ch, sizeof(char));
			sleep(3); // sleep 1 second before sending the kill to avoid race conditions problems
			kill(pid2, SIGUSR1); //send the signal to brother that there is someting to read
			pause(); // set himself to wait for the signal of read done
		}
 	}
 }
