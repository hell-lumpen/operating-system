#include <iostream>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

#define SEMKEY 88
#define SHMKEY 10
#define SEMSIZE 2

union semun {
	int val;
	semid_ds *mbuf;
	ushort *array;
	seminfo *ibuf;
} arg;

int main () {
	ushort arr[BUFSIZ];
	arg.array = arr;

	sembuf sopS[1] {{1, 1, 0}};

	sembuf sop1[2] { {0, -1, 0}, 
					 {1, 0, 0} };

	sembuf sop2[2] { {0, 0, 0 | IPC_NOWAIT}, 
					 {1, 0, 0 | IPC_NOWAIT} };

	sembuf sop3[2] { {0, 1, 0 | IPC_NOWAIT}, 
					 {1, 1, 0 | IPC_NOWAIT} };
	
	shmid_ds sbuf;
	double *p_operand1, *p_operand2;
	char *p_opt, *shm_ptr;
	int shm_desk;

	if((shm_desk = shmget(SHMKEY, (sizeof(double) * 2 + sizeof(char)), 0777)) != -1) {
		if (shmctl(shm_desk, IPC_RMID, &sbuf) == -1) {
			perror("Shmctl fail"); 
			exit(EXIT_FAILURE);
		}
	}

	if((shm_desk = shmget(SHMKEY, (sizeof(double) * 2 + sizeof(char)), 0777 | IPC_CREAT)) == -1) {
		perror("Shmget fail"); 
		exit(EXIT_FAILURE);
	}

	if (shmctl(shm_desk, SHM_LOCK, &sbuf) == -1) { 
		perror("Shmctl fail");
		exit(EXIT_FAILURE);
	}

	int sem_desk;
	if((sem_desk = semget(SEMKEY, SEMSIZE, 0777)) != -1) {
		if (semctl(sem_desk, 0, IPC_RMID, 0) == -1) {
			perror("Semctl fail");
			exit(EXIT_FAILURE);
		}
	}
	if((sem_desk = semget(SEMKEY, SEMSIZE, IPC_CREAT | 0777)) == -1) {
		perror("Semget fail"); 
		exit(EXIT_FAILURE);
	}

	if (semop(sem_desk, sopS, 1) == -1) {
		perror("Semop fail");
		exit(-5);
	}
	printf("Server has started\n");
	while (true) {
		if (semop(sem_desk, sop1, 2) == -1) {
			perror("Semop fail"); 
			exit(EXIT_FAILURE);
		}
		
		shm_ptr = (char*)shmat(shm_desk, 0, 0);
		if(shm_ptr == ((char*)-1)) {
			perror("Shmat fail"); 
			exit(EXIT_FAILURE);
		}

		p_operand1 = (double*)shm_ptr;
		p_opt = (char*)(p_operand1 + sizeof(double));
		p_operand2 = (double*)(p_opt + sizeof(char));

		if(*p_opt == '+') *p_operand1 = *p_operand1 + *p_operand2;
		else if(*p_opt == '*') *p_operand1 = *p_operand1 * *p_operand2;
		else if(*p_opt == '-') *p_operand1 = *p_operand1 - *p_operand2;
		else if(*p_opt == '/') *p_operand1 = *p_operand1 / *p_operand2;

		if(shmdt(shm_ptr) == -1) {
			perror("Shmdt fail(1)");
			exit(EXIT_FAILURE);
		}
		if (semop(sem_desk, sop2, 2) == -1) {
			perror("Semop fail");
			exit(EXIT_FAILURE);
		}
		if (semop(sem_desk, sop3, 2) == -1) {
			perror("Semop fail");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
}
