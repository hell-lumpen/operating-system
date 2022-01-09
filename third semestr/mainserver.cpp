#include "wrappers.h"

int main() {
	int server_socket = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in adr = {0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(PORT);

	Bind(server_socket, (struct sockaddr* )&adr, sizeof(adr));
	Listen(server_socket, SOMAXCONN);

	printf("The server on sockets is running\n\n");
	
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

		char msg[MSG_SIZE];

		ssize_t count_read = Recv(res_accept, msg, MSG_SIZE, 0);
		printf("recived: %s\n", msg);

		fflush(stdin);
		if (msg[0] == '1') {

			key_t key = ftok("client.cpp", 0);

			int queue_id = msgget(key, IPC_CREAT | 0666);
			if (queue_id == -1) { 
				perror("create message queue"); 
				exit(EXIT_FAILURE); 
			}

			ssize_t send = msgsnd(queue_id, msg, sizeof(msg), 0);
			if (send == -1) { 
				perror("msgsnd"); 
				exit(EXIT_FAILURE); 
			}


            ssize_t recv = msgrcv(queue_id, msg, sizeof(msg), 0, 0);
			if (recv == -1) { 
				perror("msgrcv1"); 
				exit(EXIT_FAILURE); 
			}

		}

		if (msg[0] == '2') {

			key_t 
			sem_key = ftok("server_on_shared_memory.cpp", 0);

			key_t 
			shm_key = ftok("server_on_shared_memory.cpp", 10);

			int sem_id = semget(sem_key, 2, 
				IPC_CREAT | IPC_EXCL | 0666);
			if (sem_id == -1) {
				if (errno == EEXIST) {
					sem_id = semget(sem_key, 0, 0);
					if (sem_id == -1) 
						perror("semget error");
				}
				else
					perror("semget error");
			}

            int shm_id = shmget(shm_key, MSG_SIZE, 
            	IPC_CREAT | IPC_EXCL | 0666);

			if (shm_id == -1) {

				if (errno == EEXIST) 
				{
				shm_id = shmget(shm_key, 0, 0);
				if (shm_id == -1) 
					perror("shmget error");
				}
				else
					perror("shmget error");
			}

			char* shm_ptr = (char*)shmat(shm_id, 0, 0);
			if (shm_ptr == (char*) -1) { 
				perror("shmat"); 
			}

			send_msg(shm_ptr, msg, sem_id, false);

			msg[0] = '\0';
			
			recv_msg(shm_ptr, msg, sem_id, false);

		}
		
		ssize_t 
		count_send = Send(res_accept, msg, MSG_SIZE, 0);

		printf("sended: %s\n\n", msg);
	}

	close(res_accept);
	break;
	}

	}
	
	close(server_socket);
	return 0;
}