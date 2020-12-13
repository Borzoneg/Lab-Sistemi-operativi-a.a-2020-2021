#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int atomicIncrement (int *var);

int ticketNumber;
int turnNumber;

int init(int *ticket, int *turn);
int lock(int *turn, int *ticket);
int unlock(int *turn);

int main(int argc, char **argv){
	init(&ticketNumber, &turnNumber);
	if(!fork()){
		while(1){
			lock(&turnNumber, &ticketNumber); // get the ticket and wait/pass according to current turn
			fprintf(stdout, "Father critic Section\n"); // SC
			unlock(&turnNumber); // advance the turn so other process can go furhter
			sleep(1);
		}
	}
	else{
		while(1){
			lock(&turnNumber, &ticketNumber);
			fprintf(stdout, "Child critic Section\n");
			unlock(&turnNumber);
			sleep(1);
		}
	}
	return 0;
}


int atomicIncrement (int *var){
  int tmp = *var;
  *var = tmp + 1;
  return (tmp);
}


int init(int *ticket, int *turn){ // init of values
	*ticket = 0; 
	*turn = 0;
	return 0; 
}


int lock(int *turn, int *ticket){ 
	int my_turn = atomicIncrement(ticket); // turn of the process is the current ticket, then increment ticket
	while(*turn != my_turn); // while turn of the process is different from the current turn wait
	return 0;
}


int unlock(int *turn){
	atomicIncrement(turn); // increment the turn
	return 0;
}