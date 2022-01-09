#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stddef.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	unsigned int port = 0;
	struct sockaddr_in addr;
	char name[128] = "", host_name[128] = "", 
	message_rv[1024] = "", message_sd[1024] = "";

	if (argc != 5) {
		std::cout << "you need to specify "
		"arguments in the format:"
        "\n -u ip-adress:port -a nickname\n" << std::endl;
		return -1;
	}
	
	if (std::string(argv[1]) == "-u") strcpy(name, argv[2]);
	else strcpy(name, argv[4]);
	
	if (std::string(argv[1]) == "-a") {
		for (size_t i = 0; i < strlen(argv[2]); i++) {
			if (argv[2][i] == ':') {
				host_name[i] = '\0';
				for(size_t j = i+1; j < strlen(argv[2]); j++)
					port = port * 10 + (int)(argv[2][j]) - (int)('0');

				break;
			}
			host_name[i] = argv[2][i];
		}
	}
	else
		for (size_t i = 0; i < strlen(argv[4]); i++) {
			if (argv[4][i] == ':') {
				host_name[i] = '\0';
				for(size_t j = i+1; j < strlen(argv[4]); j++)
					port = port * 10 + (int)(argv[4][j]) - (int)('0'); 

				break;
			}
			host_name[i] = argv[4][i];
		}
		
	std::cout << "nickname: " << name <<
	 "\thostname: " << host_name << 
	 "\tport: " << port << std::endl;

	ssize_t sock_b= socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sock_b < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock_b, (sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	if (inet_pton(AF_INET, host_name, &addr.sin_addr) <= 0) {
		printf("\n inet_pton error occured\n");
		exit(EXIT_FAILURE);
	}

	size_t size = sizeof(addr);
	pid_t pid = fork();

	while(true) {
		if (pid == 0) {	

			message_sd[0] = '\0';
			char buffer[1024];
			strcat(message_sd, name);
			std::cin.getline(buffer, 1024, '\n');

			strcat(message_sd, ": ");
			strcat(message_sd, buffer);

			if (sendto(sock_b, message_sd, 
				sizeof(message_sd), 0, 
				(struct sockaddr *)&addr, size) == -1) {

				perror("sendto");
				exit(EXIT_FAILURE);
			}
		}
		else if (pid > 0) {

			message_rv[0] = '\0';
			
			if (recvfrom(sock_b, message_rv, 1024, 0, 
				(struct sockaddr *)&addr, 
				(unsigned int*) &size) == -1) {

				perror("recvfrom");
				exit(EXIT_FAILURE);
			}
			
			if(strcmp(message_rv, "quit!") == 0) {
				kill(pid, SIGKILL);
				break;
			}
			
			std::cout << message_rv << std::endl;
			
			size_t index = 0;
			while(message_rv[index] != ':') index++;
			index += 2;

            if(std::string(message_rv).substr(index) == "quit!"){
				if (sendto(sock_b, "quit!", sizeof("quit!"),
				0, (struct sockaddr *)&addr, 
				size) == -1) {
					perror("sendto");
					exit(EXIT_FAILURE);
				}
				
				kill(pid, SIGKILL);
				break;
			}
		}
	}

	shutdown(sock_b, 2);
	return 0;
	
}