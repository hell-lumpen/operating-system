#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {

    if (argc != 3) { 
        std::cout << "Error arguments" << std::endl;
        return 1; 
    }

    std::ifstream fin;
    fin.open(argv[1]);

    std::string input;

    std::ofstream ofs;
    ofs.open(argv[2], std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    while (std::getline(fin, input, '\n')) {
        pid_t pid = fork();
        if (pid == 0) execl("./calc","./calc", argv[2], input.c_str(), NULL);
        if (pid < 0) { perror("fork"); exit(EXIT_FAILURE);}
        else wait(NULL);
    }
    
    fin.close();
    return 0;
}