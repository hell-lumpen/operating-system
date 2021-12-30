#include "wrappers.h"

int main(int argc, char const *argv[]) {
	int client_socket = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in adr = {0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(1111);

	Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);

	int res_connect = Connect(client_socket, (struct sockaddr* ) &adr, sizeof(adr));
	
	while (true) {

		char message[1024];

		printf("input message\n");

		fgets(message, 1024, stdin);

		if (strncmp(message, "quit!", 5) == 0) break;

		ssize_t count_send = Send(client_socket, message, 1024, 0);

		char buffer[10];

		ssize_t count_read = Recv(client_socket, buffer, 10, 0);

		printf("SERVER: The number of words in the transmitted message: %s", buffer);

	}

	close(client_socket);
	return 0;
}