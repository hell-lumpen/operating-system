#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main() {
	char dir_name[256];
	DIR* pdir = NULL;
	struct dirent *entry = NULL;

	printf(">> Input the trace of directory\n");
	fgets(dir_name, 256, stdin);
	dir_name[strlen(dir_name)-1] = '\0';

	if ((pdir = opendir(dir_name)) == NULL) {
		printf(">> Unable to open this directory\n");
		return -1;
	}

	while((entry = readdir(pdir)) != NULL) {
		printf("%s\n", entry->d_name);
	}

	closedir(pdir);

	return 0;
}