#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

double calc(const char *num1, char oper, const char *num2) {

    if (oper == '+') return atof(num1) + atof(num2);
    if (oper == '-') return atof(num1) - atof(num2);
    if (oper == '*') return atof(num1) * atof(num2);
    if (oper == '/') return atof(num1) / atof(num2);

    std::cerr << 
    "undefined operator \"" << oper << "\"" 
    << std::endl;
    exit(EXIT_FAILURE);
    return -1;
}


int main(int argc, char* argv[]) {

    std::ofstream out;
    out.open(argv[1], std::ios::app);

    if (out.is_open()) {
    char operator_;
    size_t index;

    for (size_t i = 0; i < strlen(argv[2]); i++) {
        if (argv[2][i] == '+' || argv[2][i] == '-' ||
         argv[2][i] == '*' || argv[2][i] == '/') {
            operator_ = argv[2][i];
            index = i;
            break;
        }
    }

    std::string 
    number1 = std::string(argv[2]).substr(0, index - 1);

    std::string 
    number2 = std::string(argv[2]).substr(index + 1);

    if (atof(number2.c_str()) == 0 && operator_ == '/') {
        std::cerr << "divizion by zero" << std::endl;
        out << number1 + operator_ + number2 << 
        " = " << "divizion by zero" << std::endl;
    }

    else {
        std::cout << 
        calc(number1.c_str(), operator_, number2.c_str()) 
        << std::endl;
        out << number1 + operator_ + number2 << " = " << 
        calc(number1.c_str(), operator_, number2.c_str()) 
        << std::endl;
    }
    }
    else {
        perror("open file");
        exit(EXIT_FAILURE);
    }
    
    out.close();
    return 0;   
}