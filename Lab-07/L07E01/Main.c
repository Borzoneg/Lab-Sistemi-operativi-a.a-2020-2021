/*
 *  Print-out binary numbers with concurrency
 *  Output order fixed <-> Reduced concurrency
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main (
  int argc,
  char *argv[]
)
{
  int i,j,n, *vet;
  int retValue;
  pid_t pid;

  if (argc<2) {
    fprintf (stderr, "Run  it as: pgrm n\n");
    exit (1);
  }

  n = atoi (argv[1]);

  vet = (int *) malloc (n * sizeof (int));
  if (vet==NULL) {
    fprintf (stderr, "Allocation Error.\n");
    exit (1);
  } 

  fprintf (stdout, "Output:\n");
  for (i=0; i<=n; i++) {

    if (i == n) {
      for (j=0; j<n; j++) {
        printf("%d", vet[j]);
      }
     printf("\n");
     exit (0);

    } else {

      vet[i] = 0;    
      if (fork()) {
	   // Father
         pid = wait (&retValue);
         vet[i] = 1; 
         if (fork()) {                
           // Father
           pid = wait (&retValue);
           exit (0);
         }  // if
      } // if

    } // else

  } // for

  //fprintf (stdout, "vet[%d]:%d - ", i, vet[i]);
  //fprintf (stdout, "End PID=%d\n", getpid());
  return (0);
}
