#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "analyzer.h"

int main(int argc, char** argv) {

	key_t key = ftok("client.cpp", 0);

	int queue_id = msgget(key, IPC_CREAT | 0666);

	if (queue_id == -1) { 
		perror("create message queue"); 
		exit(EXIT_FAILURE); 
	}

	printf("The server on message queue is running\n\n");

	while (true) {

		pid_t pid;
		if ((pid = fork()) == -1) {
			perror("fork");
			continue;
		}
		if (pid == 0){ 

			printf("FORK\n");

			char msg[1024];
			std::map<char, size_t> dictionary;

			
			while (true)
			{

			ssize_t 
			recv = msgrcv(queue_id, msg, sizeof(msg), 0, 0);
			
            if (recv == -1) { 
            	perror("msgrcv"); 
            	exit(EXIT_FAILURE); 
            }
	
			printf("recived: %s\n", msg);
			const 
			char* reply = analiz(msg, dictionary);
		
			ssize_t 
			send = msgsnd(queue_id, reply, sizeof(msg), 0);	
			if (send == -1) { 
				perror("msgsnd"); 
				exit(EXIT_FAILURE); 
			}
	
			print_gistogram(dictionary);
		
			printf("sended: %s\n\n", reply);
			
			}
		}
		if (pid > 0) {
			wait(NULL);
		}
	}

	msgctl(key, IPC_RMID, NULL);
	return 0;
}