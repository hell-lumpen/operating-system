#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <time.h>

using namespace std;
#define SIZE 128

// Создать символическую ссылку на файл с текстом программы по указанному пути и 
// вывести информацию по ней об атрибутах данной символической ссылки.

// макрос __FILE__ содержит расположение файла исходного кода
// symlink() создает символьную ссылку с именем newpath, которая содержит строку oldpath.
// Символьная ссылка (также известная как мягкая ссылка) может указывать на существующий файл или на несуществующий.
// При успехе возвращается ноль. При ошибке возвращается -1, а errno задается соответствующим образом.  

// struct stat {
//     dev_t         st_dev;      /* устройство */
//     ino_t         st_ino;      /* inode */
//     mode_t        st_mode;     /* режим доступа */
//     nlink_t       st_nlink;    /* количество жестких ссылок */
//     uid_t         st_uid;      /* идентификатор пользователя-владельца */
//     gid_t         st_gid;      /* идентификатор группы-владельца */
//     dev_t         st_rdev;     /* тип устройства (если это устройство) 
//     off_t         st_size;     /* общий размер в байтах */
//     blksize_t     st_blksize;  /* размер блока ввода-вывода */
//                                /* в файловой системе */
//     blkcnt_t      st_blocks;   /* количество выделенных блоков */
//     time_t        st_atime;    /* время последнего доступа */
//     time_t        st_mtime;    /* время последней модификации */
//     time_t        st_ctime;    /* время последнего изменения */
// };


void display_file_type ( int st_mode ) 
{
    printf("2. Type: ");                                   
    switch ( st_mode & S_IFMT )
    {
        case S_IFDIR:  printf("directory\n"); return;
        case S_IFCHR:  printf("symbolic device\n"); return;
        case S_IFBLK:  printf("blok devise\n"); return;
        case S_IFREG:  printf("common file\n"); return;
        case S_IFLNK:  printf("link\n"); return;
        case S_IFSOCK: printf("socket\n"); return;
    }
} 
 

void display_permission ( int st_mode )
{
  static const char xtbl[11] = "rwxrwxrwx\n";
  char     amode[11];
  int      i, j;
 
  for ( i = 0, j = ( 1 << 8 ); i < 9; i++, j >>= 1 )
    amode[i] = ( st_mode&j ) ? xtbl[i]: '-';
  if ( st_mode & S_ISUID )   amode[2]= 's';
  if ( st_mode & S_ISGID )   amode[5]= 's';
  if ( st_mode & S_ISVTX )   amode[8]= 't';
  amode[9]='\0';
  printf ("3. Access mode: %s\n",amode );
}


void print_info(char* path_name) {
  printf("LINK INFORMATION:\n");
  struct stat     statv;
  struct passwd  *pw_d;
  char *date;
 
  if ( lstat ( path_name, &statv ) )
  { 
    perror ( path_name ); 
    return;
  }
  printf("1. Name: %s\n", path_name);
  display_file_type ( statv.st_mode );
  display_permission ( statv.st_mode );
  printf("4. Inode number: %li\n", statv.st_ino);
  printf ("5. Count of hardlink: %lu \n",statv.st_nlink );  // значение счетчика жестких связей
  pw_d = getpwuid ( statv.st_uid ); // преобразовать UID в имя пользователя
  printf ("6. User ID: %s \n",pw_d->pw_name );   // и напечатать его
  printf("7. Group ID: %d \n",statv.st_gid);
  printf ("8. Size: %ld \n", statv.st_size );
  char* ctime = asctime(localtime(&statv.st_ctime));
  char* atime = asctime(localtime(&statv.st_atime));
  char* mtime = asctime(localtime(&statv.st_mtime));
  printf("9. Time of last access: %s", atime);
  printf("10. Time of last modification: %s", mtime);
  printf("11. Time of last status change: %s", ctime);
}


int main(int argc, char* argv []){
  const char* name = __FILE__;
  char* linkname = strcat(argv[1], name);
  linkname = strcat(linkname, ".link");

  if (int file = open(name, O_WRONLY) != -1) {
    if(symlink(name, linkname) == 0){
      printf("link \"%s\" was created \n", linkname);
      print_info(linkname);
    }
    else{
      perror("link error");
      if (errno == 17)
      {
        print_info(linkname);
      }
    }
  }
  
  return 0;
}
