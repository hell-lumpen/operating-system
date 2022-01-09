#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string>
#include <map>
#include <iostream>
#include <errno.h>

#define PORT 3222
#define MSG_SIZE 1024

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
		perror("send");
		exit(EXIT_FAILURE);
	}

	return res;
}

enum SEM_OP {
    RESET_SEM, CLIENT_GET_MEM, 
    CLIENT_SND_REQ, SERVER_RCV_REQ,
    SERVER_GET_MEM, SERVER_SND_RPY, 
    CLIENT_RCV_RPY
};

void change_semaphores(int semId, SEM_OP type) {

	static struct sembuf args[2] = {{0}, {1}};
    
    switch (type) {

        case SERVER_GET_MEM:
        	return;

        case CLIENT_GET_MEM:
            args[0].sem_op = 0,
            args[1].sem_op = -1;
            break;

        case CLIENT_SND_REQ:
            args[0].sem_op = 1,
            args[1].sem_op = 0;
            break;

        case SERVER_RCV_REQ:
            args[0].sem_op = -1,
            args[1].sem_op = 0;
            break;

        case SERVER_SND_RPY:
            args[0].sem_op = 1,
            args[1].sem_op = 1;
            break;

        case CLIENT_RCV_RPY:
            args[0].sem_op = -1,
            args[1].sem_op = -1;
            break;

        case RESET_SEM:
            args[0].sem_op = 0,
            args[1].sem_op = 1;
            break;
    }

    if (semop(semId, args, 2) == -1)
        perror("semop");
}


// union semun  {
//     int val;
//     struct semid_ds *buf;
//     ushort *array;
// } arg;

// void print_value(int sem_id) {
// 	int val0 = semctl(sem_id, 0, GETVAL, arg);
// 	int val1 = semctl(sem_id, 1, GETVAL, arg);
// 	printf("sem_1 = %d\nsem_2 = %d\n", val0, val1);	
// }

void send_msg(char* shm_ptr, char* msg, int sem_id, bool is_server) 
{
	change_semaphores(sem_id, 
		is_server ? SERVER_GET_MEM : CLIENT_GET_MEM);
	memcpy(shm_ptr, msg, MSG_SIZE); // memcpy(dest, source, len);
	change_semaphores(sem_id, 
		is_server ? SERVER_SND_RPY : CLIENT_SND_REQ);
}

void send_msg(char* shm_ptr, const char* msg, int sem_id, bool is_server) 
{
	change_semaphores(sem_id, 
		is_server ? SERVER_GET_MEM : CLIENT_GET_MEM);
	memcpy(shm_ptr, msg, MSG_SIZE); // memcpy(dest, source, len);
	change_semaphores(sem_id, 
		is_server ? SERVER_SND_RPY : CLIENT_SND_REQ);
}

void recv_msg(char* shm_ptr, char* msg, int sem_id, bool is_server) 
{
	change_semaphores(sem_id, 
		is_server ? SERVER_RCV_REQ : CLIENT_RCV_RPY);
	memcpy(msg, shm_ptr, MSG_SIZE);
	if (!is_server) 
		change_semaphores(sem_id, RESET_SEM);
}
