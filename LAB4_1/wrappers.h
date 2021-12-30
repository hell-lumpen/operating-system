#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int Socket(int domain, int type, int protokol) {
	int res = socket(domain, type, protokol);
	if (res == -1) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	return res;
}


void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
	int res = bind(sockfd, addr, addrlen);
	if (res == -1) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

void Listen(int sockfd, int backlog) {
	int res = listen(sockfd, backlog);
	if (res == -1) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
	int res = accept(sockfd, addr, addrlen);
	if (res == -1) {
		perror("accept error");
		exit(EXIT_FAILURE);
	}

	return res;
}

void Inet_pton(int af, const char *src, void *dst) {
	int res = inet_pton(af, src, dst);

	if (res == 0) {
		printf("inet_pton failed unvalid netvork address\n");
		exit(EXIT_FAILURE);
	}

	if (res == -1) {
		perror("inet_pton failed");
		exit(EXIT_FAILURE);
	}
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int res = connect(sockfd, addr, addrlen);

	if (res == -1) {
		perror("connect error");
		exit(EXIT_FAILURE);
	}

	return res;
}

ssize_t Recv(int sockfd, void *buf, size_t len, int flags) {
	int res = recv(sockfd, buf, len, flags);

	if (res == -1) {
			perror("read failed");
			exit(EXIT_FAILURE);
	}

	if (res == 0) {
		printf("EOF\n");
	}

	return res;
}

ssize_t Send(int sockfd, const void *buf, size_t len, int flags) {
	int res = send(sockfd, buf, len, flags);

	if (res == -1) {
		perror("send:");
		exit(EXIT_FAILURE);
	}

	return res;
}