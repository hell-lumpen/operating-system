#include "wrappers.h"

size_t words_counter(char *buffer) {
	if (strlen(buffer) == 0) return 0;

	size_t count = 0;
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] == ' ' || buffer[i] == '\t')
			count++;
	}

	return count + 1;
}


int main() {
	int server_socket = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in adr = {0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(1111);

	Bind(server_socket, (struct sockaddr* )&adr, sizeof(adr));
	Listen(server_socket, 1);

	socklen_t adr_len = sizeof(adr);

	int res_accept = Accept(server_socket, (struct sockaddr* ) &adr, &adr_len);

	while(true) {
		char buffer[1024];
		ssize_t count_read = Recv(res_accept, buffer, 1024, 0);
		printf("%s\n", buffer);
		fflush(stdin);

		char res[10];
		int result = words_counter(buffer);
		sprintf(res, "%d\n", result);

		ssize_t count_send = Send(res_accept, res, 10, 0);

	}

	sleep(1);
	close(server_socket);
	close(res_accept);

	return 0;
}