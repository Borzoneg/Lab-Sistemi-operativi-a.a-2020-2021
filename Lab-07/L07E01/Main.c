#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int signal_flag; // flag for signal handler 1 = SIGUSR1, 2 = SIGUSR 2

void binary(int *vet, int lenght, int pos);

void manager(int SIG){
	if(SIG == SIGUSR1){
		signal_flag = 1;
		return;
	}
	if(SIG == SIGUSR2){
		signal_flag = 2;
		return;
	}
}

int main(int argc, char ** argv){

	int n, *vet;

	if(argc != 2){
		fprintf(stderr, "Not enough arguments\n");
		exit(1);
	}

	n = atoi (argv[1]);
	vet = (int *)malloc(n * sizeof(int));

	if (vet == NULL) {
		fprintf(stderr, "Allocatin Error.\n");
		exit(1);
	}

	fprintf(stdout, "Binary Numbers:\n");
	binary(vet, n, 0);
	free(vet);

	return 0;
}

void binary (int *vet, int lenght, int pos){
	
	int i, j, pids[lenght], pid_pre, n_bin = 1, pipe_extr[2], n = 0;
	
	pipe(pipe_extr);
	signal(SIGUSR1, manager); // instance signal manager
	signal(SIGUSR2, manager); // instance signal manager

	for(i=0; i<lenght; pid_pre=pids[i], i++, n_bin*=2){
		pids[i] = fork();
		if(!pids[i]){ // son case
			close(pipe_extr[0]);
			write(pipe_extr[1], &n, sizeof(int));
			while(1){
				pause();
				if(signal_flag == 2)
					write(pipe_extr[1], &n, sizeof(int));
				else{
					n = n == 0 ? 1 : 0;
					write(pipe_extr[1], &n, sizeof(int));
					for(j=i-1; i>=0; i--)
						kill(pids[i], SIGUSR2); 
					
				}
			}
		}
	}

	for(i=0; i<n_bin; i++){
		for(j=0; j<lenght; j++){
			read(pipe_extr[0], &n, sizeof(int));
			fprintf(stdout, "%d", n);
		}
		fprintf(stdout, "\n");
		kill(pids[lenght-1], SIGUSR1);	
	}
	return;
}



//   if (pos >= lenght) {
//     for (j=0; j<lenght; j++) {
//       printf("%d", vet[j]);
//     }
//     printf("\n");
//     return;
//   }

//   vet[pos] = 0;
//   binary(vet, lenght, pos+1);

//   vet[pos] = 1;
//   binary(vet, lenght, pos+1);

//   return;
// }




