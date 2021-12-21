#include <iostream>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <filesystem>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;

// Вывести на экран информацию обо всех файлах в директории, отсортированную по 
// времени последнего изменения файла.

// Поля стрктуры dirent

// struct dirent {
//   ino_t          d_ino;       inode number
//   off_t          d_off;       offset to the next dirent
//   unsigned short d_reclen;    length of this record
//   unsigned char  d_type;      type of file; not supported
//                                  by all file system types
//   char           d_name[256]; filename
// };


bool comparator(const pair<string,time_t> &a, const pair<string, time_t> &b) {
	return (a.second < b.second);
}

int main(int argc, char* argv[]) {
	time_t t = time(0);
	string dir_name;
	string file_name;

	vector<pair<string, time_t>>vector_of_file;
	cout << "Enter the path to the directory\n" << endl;
	cin >> dir_name;
	cout << endl;

	struct stat stats;
	struct timespec ts;

	DIR* dir = opendir(dir_name.c_str());
	struct dirent* directory;

	while((directory = readdir(dir)) != 0) {
		file_name = directory->d_name;
		stat(file_name.c_str(), &stats);
		time_t mod_time = stats.st_mtim.tv_sec;
		vector_of_file.push_back(make_pair(file_name, mod_time));
		sort(vector_of_file.begin(),vector_of_file.end(), comparator);
	}

	for(auto &i :vector_of_file
){
		cout << "File name: " << i.first << "\t Time of last modification: " << asctime(localtime(&i.second)) << endl;
	}
}
