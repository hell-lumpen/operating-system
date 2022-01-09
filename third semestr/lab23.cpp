#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char* 
magic(const char* filename, char* linkname, 
  int index, int i, int j) {

  linkname = (char*) malloc(sizeof(char) * (strlen(filename)));
  char buffer[5];

  for (i = 0; filename[i] != '.'; i++)
    linkname[i] = filename[i];

  linkname[i] = filename[i];

  sprintf(buffer, "%d", index);
  strcat(linkname, buffer);

  j = i + strlen(buffer) + 1;
  linkname[j] = '.';
  for (; i <= strlen(filename); ++i, ++j)
    linkname[j] = filename[i];

  linkname[j+1] = '\0';
  return linkname;
}
 
int main(int argc, char const *argv[])
{
  if (argc == 1)
  {
    perror ("not enough arguments");
    exit(-1);
  }
  else if (argc == 2)
  {
    char* filename = 
    (char*) malloc(sizeof(char) * (strlen(argv[1]) + 1));
    char* linkname;
    int index = 1;

    int i, j;
    for (i = 0; i < strlen(argv[1]); ++i)
    {
      filename[i] = argv[1][i];
    }
    printf("%s\n", argv[0]);

    if (int file = open(filename, O_WRONLY) != -1) {
      printf("file is open %s\n", filename);

      while (link(filename, linkname = 
        magic(filename, linkname, index, i, j)) == -1)
        index++;

      printf("link is created: %s\n", linkname);
      close(file);
      free(filename);
      free(linkname);
    }
    else {
      perror ("open file error");
      exit(-1);
    }

  }

  else
  {
    perror ("too many arguments");
    exit(-1);
  }
  return 0;
}