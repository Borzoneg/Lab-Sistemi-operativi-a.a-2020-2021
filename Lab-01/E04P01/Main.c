#include <stdio.h>

int main(int argc, char **argv){
  char buf[1024];
  FILE *src = fopen(argv[1], "r");
  FILE *dest = fopen(argv[2], "w");
  while((fgets(buf, 132, src))!=NULL){
     fputs(buf, dest);
  }
  fclose(src);
  fclose(dest);
  return(0);
}
