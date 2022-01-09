#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <string.h>

void print_gistogram(std::map<char, size_t>&);

void count_symbols(std::map<char, size_t>&, std::string &);

void 
map_initialization(std::map<char, size_t>& dict, std::string &filename) {
    dict.clear();
    std::ifstream in(filename);
    std::string line;
    char ch;
    int count;
    if (in.is_open()) {
        while (getline(in, line)) {
            sscanf(line.c_str(), "%c %d", &ch, &count);
            dict.insert(std::make_pair(ch, count));
        }
    }
    
    in.close();
}

const char* 
analiz(char *pathname, std::map<char, size_t> &dictionary) {
	std::string line;

    size_t i;
    for (i = 0; pathname[i] != '\0'; i++) {
        if (pathname[i] == ' ') 
            break;
    }

    std::string 
    file_for_stat = std::string(pathname).substr(1, i - 1);
    std::string 
    user_file = std::string(pathname).substr(i + 1);

    std::cout << "pid: " << file_for_stat << 
    " user_file: " << user_file << std::endl;

    map_initialization(dictionary, file_for_stat);
    std::ifstream in(user_file);
    if (in.is_open())
    {
        while (getline(in, line)) {
            count_symbols(dictionary, line);
        }
    }
    else
    	return "File not found ";

    in.close();

    std::ofstream out;
    out.open(file_for_stat);

    if (out.is_open())
    {
        for (auto &el : dictionary) 
        	out << el.first << " " << el.second << std::endl;
    }
    else
    	std::cout << "file open error\n" << std::endl;

    out.close();

    return "Done";
}

void print_gistogram(std::map<char, size_t> &dictionary) {
	for (auto &elem : (dictionary)) {
		std::cout << "symbol \"" << elem.first << "\"\t";
		for (size_t i = 0; i < elem.second; ++i) {
			std::cout << "#";
		}
		std::cout << " (" << elem.second << ")" << std::endl;
	}
}

void print_file_gistogram(char* pathname) {
	std::ifstream in(pathname);
	std::string line;
	char ch;
	int count;
    if (in.is_open())
    {
        while (getline(in, line)) {
            sscanf(line.c_str(), "%c %d", &ch, &count);
            std::cout << "symbol \"" << ch << "\"\t";
            for (size_t i = 0; i < count; ++i) {
            	std::cout << "#";
            }
            std::cout << " (" << count << ")" << std::endl;
        }
    }
    else
    	printf("Statistics are not available" 
            "(the requested file was not found)\n");
}

void 
count_symbols(std::map<char, size_t> &dictionary, std::string &str) {
	for (auto &pchar : (str)) {
		if (pchar != '\n' || pchar != '\t' || pchar != ' ') {
			auto find_pair = dictionary.find(pchar);
            if (find_pair != dictionary.end())
                find_pair->second++;
            dictionary.insert(std::pair<char, size_t>(pchar, 1));
		}
	}
}