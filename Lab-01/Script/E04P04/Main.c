#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFDIM 1024

int main(int argc, char **argv){
  char buf[BUFDIM];
  int oldfd, newfd;
  int count;

  if(argc != 3){
    fprintf(stderr, "Usage: %s, oldfile newfile\n", argv[0]);
    exit(-1);
  }

  oldfd = open(argv[1], O_RDONLY);
  if(oldfd == -1){
    fprintf(stderr, "%s: cannot open file %s\n", argv[0], argv[1]);
    exit(-1);
  }

  newfd = creat(argv[2], 0666);
  if(newfd == -1){
    fprintf(stderr, "%s: cannot open file %s\n", argv[0], argv[2]);
    exit(-1);
  }

  while((count = read(oldfd, buf, BUFDIM)) > 0){
    write(newfd, buf, count);
  }

  close(newfd);
  close(oldfd);

  return(0);
}
