#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int copyFile(char * path, char *newRoot);
int copyDirectory(char * path, char *newRoot);


int main(int argc, char **argv){
	struct stat root;

	if(stat(argv[1], &root) < 0){
		fprintf(stdout, "stat error.\n");
		exit(1);
	}
	
	if(S_ISDIR(root.st_mode))
		copyDirectory(argv[1], argv[2]);     
	   
	else
		copyFile(argv[1], argv[2]);

}


int copyDirectory(char * path, char *newRoot){
	DIR *dp  = opendir(path);
	struct dirent *dirp;
	struct stat buf;
	char fullname[257], newPath[257];
	if(dp == NULL){
		fprintf(stdout, "Unable to open directory\n");
		exit(1);
	}

	sprintf(newPath, "%s/%s", newRoot, path);

	if(mkdir(newPath, 0700) < 0){
		fprintf(stderr, "Error in creating the directory\n");
		exit(1);
	}

	while((dirp = readdir(dp)) != NULL){
		if(strcmp(".", dirp->d_name) && strcmp("..", dirp->d_name)){ //Check if it's a directory different from itself and the father ;
			sprintf(fullname, "%s/%s", path, dirp->d_name); // Creating the whole name for the file inside the folder
			if(stat(fullname, &buf) < 0){ // Error reading the new file
				fprintf(stderr, "Error in reading file inside folder\n");
				exit(1);
			}

			if(S_ISDIR(buf.st_mode)){ // If is a dir
				copyDirectory(fullname, newRoot);
			}
			else
				copyFile(fullname, newRoot);
		}
	}
	closedir(dp);
	return(0);
}


int copyFile(char * filename, char *newRoot){
	char buf[100];
	sprintf(buf, "%s/%s", newRoot, filename);
	FILE *fp = fopen(filename, "r"), *fd = fopen(buf, "w");

	while((fgets(buf, 100, fp)) != NULL){
		fputs(buf, fd);
	}
	fclose(fp);
	fclose(fd);
	return(0);
}
