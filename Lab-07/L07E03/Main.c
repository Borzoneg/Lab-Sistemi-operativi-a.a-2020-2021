#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
	
	int i, n = 5, j, tmp;
	int v[]  = {102, 99, 34, 234, 25};

	fprintf(stdout, "%d %d %d %d %d\n", v[0], v[1], v[2], v[3], v[4]);

	for(i=0; i<n; i++){
		for(j=0; j<n-i; j++){
			if(v[j] > v[j+1]){
				tmp = v[j];
				v[j] = v[j+1];
				v[j+1] = tmp;
			}
		}
	}

	fprintf(stdout, "%d %d %d %d %d\n", v[0], v[1], v[2], v[3], v[4]);
}