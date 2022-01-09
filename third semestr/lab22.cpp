#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


// Поля стрктуры dirent


// struct dirent {
//   ino_t          d_ino;       inode number
//   off_t          d_off;       offset to the next dirent
//   unsigned short d_reclen;    length of this record
//   unsigned char  d_type;      type of file; not supported
//                                  by all file system types
//   char           d_name[256]; filename
// };


int main() {
	char dir_name[256];
	DIR* pdir = NULL;
	struct dirent *entry = NULL;

	printf("Input the trace of directory\n>>");
	fgets(dir_name, 256, stdin);
	dir_name[strlen(dir_name)-1] = '\0';

	if ((pdir = opendir(dir_name)) == NULL) {
		printf("Unable to open this directory\n");
		return -1;
	}

	while((entry = readdir(pdir)) != NULL) {
		if (entry->d_type != DT_DIR)
		{
			printf("%s\n", entry->d_name);
		}
	}

	closedir(pdir);

	return 0;
}