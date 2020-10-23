#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int printFile(char * path);
int visitDirectory(char * path);


int main(int argc, char **argv){
	struct stat root;

	if(stat(argv[1], &root) < 0){
		fprintf(stdout, "stat error.\n");
		exit(1);
	}
	
	if(S_ISDIR(root.st_mode))
		visitDirectory(argv[1]);     
	   
	else
		printFile(argv[1]);

}


int visitDirectory(char * path){
	DIR *dp  = opendir(path);
	struct dirent *dirp;
	struct stat buf;
	char fullname[257];
	char directoriesNames[50][257];
	int nDir = 0, i;
	
	if(dp == NULL){
		fprintf(stdout, "Unable to open directory");
		exit(1);
	}
	
	fprintf(stdout, "In the root %s there are:\n", path);

	while((dirp = readdir(dp)) != NULL){
		if(strcmp(".", dirp->d_name) && strcmp("..", dirp->d_name)){ //Check if it's a directory different from itself and the father ;
			sprintf(fullname, "%s/%s", path, dirp->d_name); // Creating the whole name for the file inside the folder
			if(stat(fullname, &buf) < 0){ // Error reading the new file
				fprintf(stderr, "Error in reading file inside folder");
				exit(1);
			}

			if(S_ISDIR(buf.st_mode)){ // If is a dir
				fprintf(stdout, "%s: directory\n", dirp->d_name); // printing info on the file/directory   
				strcpy(directoriesNames[nDir], fullname); // Store the name of the directory for later
				nDir++; // update the number of directories stored
			}
			else{
				fprintf(stdout, "%s: file\n", dirp->d_name); // printing info on the file/directory   
				printFile(fullname);
			}
		}
	}
	fprintf(stdout, "\n");

	closedir(dp);
	dp = opendir(path);

	for(i=0; i<nDir; i++){
		visitDirectory(directoriesNames[i]); // visit each directory 
	}
	return(0);
}


int printFile(char * filename){
	FILE *fp = fopen(filename, "r");
	char buf[100];
	fprintf(stdout, "File: %s\n", filename);
	while((fgets(buf, 100, fp)) != NULL)
		fprintf(stdout, "%s", buf);
	fclose(fp);
	fprintf(stdout, "\n");
	return(0);
}
