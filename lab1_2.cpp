#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/random.h>
#include <time.h>
#include <map>
#include <fstream>
using namespace std;

int generate_random() {
    unsigned long x;
    getrandom(&x, sizeof(unsigned long), 0);
    printf("%ld\n", x%100);
    if (x % 100 <= 20) return 0;
    else if (x % 100 <= 65) return 1;
    else if (x % 100 <= 90) return 2;
    else if (x % 100 <= 100) return 3;
}

void generate_process(int counter, bool mainFlag, char* FName) {

    int random = generate_random();

    if (random != 0) {
        for (int i = 0; i < random; i++) {  
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                generate_process(counter + 1, 0, FName);
                break;
            }
            else if (pid > 0) {
                wait(nullptr);
            }
        }
    }
    
    ofstream of(FName, std::ios::app);
    of << counter << endl;
    of.close();

    if (!mainFlag) _exit(0);
}

int main() {
    srand(time(NULL));
    char* name = "tmp.txt";

    std::ofstream ofs;
    ofs.open(name, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    generate_process(1, true, name);
    ifstream f(name);
    std::map<int,int> _map;
    string ch;

    while(getline(f,ch)) {
        _map[atoi(ch.c_str())] += 1;
    }

    cout << "number of parents\t" << "the number of such processes" << endl;

    for (auto &obj : _map) {
        cout << "\t" << obj.first << "\t\t\t";

        for(int i = 0; i <  obj.second;i++)
            cout << "#";

        cout << " (" << obj.second << ")" << endl;
    }
    f.close();
    return 0;
}