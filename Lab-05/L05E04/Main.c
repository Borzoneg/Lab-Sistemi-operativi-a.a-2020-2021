#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX 20

void manager(int SIG){
	if(SIG == SIGUSR1){
		fprintf(stdout, "SIGUSR1 RECEIVED\n");
		return;
	}
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
			write(pipe_w, &pid2, sizeof(pid2)); // write the pid2 on the pipe to give it to 1st son
			sleep(3); // sleep 1 for race conditions problems with first son
			kill(pid, SIGUSR1); // wake up the first son
			close(pipe_r);
			close(pipe_w);
			pause();			
		}

		else{ // 2nd son case
			close(pipe_w); // close one part of the pipe
			fprintf(stdout, "My [Son2] Brother pid: %d\n", pid);
			while(1){
				pause(); // wait for the signal of read done form stdout
				while((n = read(pipe_r, &ch, sizeof(char))) > 0){ // read char by char from pipe
					ch -= 32; // convert into capitalized letter
					setbuf(stdout, NULL);
					fprintf(stdout, "%c", ch);	// print char by char on stdout
				}
				// TODO: doesn't reach this point
				setbuf(stdout, NULL);
				fprintf(stdout, "\nNuova parola:\n");
				sleep(3); // sleep 1 for race conditions problems
				kill(pid, SIGUSR1); //send the signal to the brother that confirm read of the data
			}	
		}
	}
	else{
		pause(); // wait for the father to write the value of the pid 2 on the pipe
		read(pipe_r, &pid2, sizeof(pid2)); // read the value of pid2 from the father
		fprintf(stdout, "My [Son1] Brother pid: %d\n", pid2);
		close(pipe_r); // close one part of the pipe
		sleep(3);
		while(1){
			fscanf(stdin, "%s", buf);
			write(pipe_w, buf, sizeof(char) * strlen(buf)); // write char on pipe
			sleep(3); // sleep 1 second before sending the kill to avoid race conditions problems
			kill(pid2, SIGUSR1); //send the signal to brother that there is someting to read
			pause(); // set himself to wait for the signal of read done
		}
 	}
 }
