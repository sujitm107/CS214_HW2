#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

void find(char* searchStr, char* directory, char* path){

	// List the contents of the directory -- readdir? Or should we use ls and pipe?

	
	struct dirent* dir;
	DIR* dirp= opendir(directory);
	errno = 0;

	// Iterate through the contents
	while((dir = readdir(dirp)) != NULL) {
		//if it's a directory make a recursive call.
		if(dir->d_type == DT_DIR){
			//concatenate the string
			if(strcmp(dir->d_name, ".") == 0){
				continue;
			}
			if(strcmp(dir->d_name, "..") == 0){
				continue;
			}
			if(strcmp(dir->d_name, ".git") == 0){
				continue;
			}

			char* newPath = malloc((strlen(path) + strlen(dir->d_name)) * sizeof(char));
			newPath = strcat(newPath, path);
			newPath = strcat(newPath, dir->d_name);
			newPath = strcat(newPath, "/");

			find(searchStr, dir->d_name, newPath);

			free(newPath);
		} else {
			if(strstr(dir->d_name, searchStr)){
				printf("%s%s\n", path, dir->d_name);
			}
		}
	}

	closedir(dirp);

	if(errno)
		printf("error!");
}

int main(int argc, char **argv){

	// Get Substring argv[0]
	char* searchStr = argv[1];

	//get stats on current dirrectory
	find(searchStr, ".", "./");

	return 0;

}
