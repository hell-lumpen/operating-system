#include "wrappers.h"

int main() {
	int server_socket = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in adr = {0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(PORT);

	Bind(server_socket, (struct sockaddr* )&adr, 
		sizeof(adr));
	Listen(server_socket, SOMAXCONN);
	
	while (true) {
		socklen_t adr_len = sizeof(adr);
		int res_accept = Accept(server_socket, 
			(struct sockaddr* ) &adr, &adr_len);
		pid_t pid;

		if ((pid = fork()) == -1) {

			perror("fork");
			close(res_accept);
			continue;

		} else if (pid > 0) {

			close(res_accept);
			continue;

		} else if (pid == 0) {

			while(true) {

			char buffer[1024];
			ssize_t 
			count_read = Recv(res_accept, 
				buffer, 1024, 0);
			printf("%s\n", buffer);
			fflush(stdin);
			if (buffer[0] == '1') {
				
			}

			if (buffer[0] == '2') {
				
			}
			
			ssize_t count_send = Send(res_accept, 
				"RESULT\n", sizeof("RESULT\n"), 0);
			}

			close(res_accept);
			break;
		}
	}
	
	close(server_socket);
	return 0;
}