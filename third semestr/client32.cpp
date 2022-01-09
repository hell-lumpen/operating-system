#include <iostream>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string>

using namespace std;

#define SEMKEY 5
#define SHMKEY 10
#define SEMSIZE 2

bool is_double(string str) {
	char *err_checker = nullptr;
	double value = strtod(str.c_str(), &err_checker);

	if(err_checker == str.c_str() || *err_checker != '\0') 
		return 0;
	return 1;
}

bool is_operation(string str) {

	if(str.length() != 1) return 1;

	if(str[0] != '+' && str[0] != '-' &&
	 str[0] != '/' && str[0] != '*') 
		return 0;

	return 1;
}

// union semun
// {
// 	int val;
// 	semid_ds *mbuf;
// 	ushort *array;
// 	seminfo *ibuf;
// } arg;

int main () {
	// ushort arr[BUFSIZ];
	// arg.array = arr;

	sembuf sop1[2] {{0, 0, 0}, {1, -1, 0}};
	sembuf sop2[2] {{0, 1, 0 | IPC_NOWAIT}, 
	{1, 0, 0 | IPC_NOWAIT}};
	sembuf sop3[2] {{0, -1, 0}, {1, -1, 0}};
	sembuf sop4[2] {{0, 0, 0 | IPC_NOWAIT}, 
	{1, 1, 0 | IPC_NOWAIT}};
	shmid_ds sbuf;

	double *p_operand1, *p_operand2;
	char *p_opt;
	string tmpd1, tmpd2, tmpc;
	char *shm_ptr;
	int shm_d;

	while(true) {
		cout << "Enter your expression "
		"(number operator number):\n>>> ";
		cin >> tmpd1;
		if (tmpd1 == "quit") {
			cout << "program terminated" << endl;
			break;
		}
		cin >> tmpc;
		cin >> tmpd2;

		if(!is_double(tmpd1) || 
			!is_operation(tmpc) || !is_double(tmpd2)) {
			cout << "Invalid enter, try again" << endl;
			continue;
		}

		if((shm_d = shmget(SHMKEY, 
			(sizeof(double) * 2 + sizeof(char)),
		    0777 | IPC_CREAT)) == -1) {
			
			perror("Shmget fail");
			exit(EXIT_FAILURE);
		}

		int sem_d;
		if((sem_d = semget(SEMKEY, SEMSIZE, 0777)) == -1) {
			perror("Semget fail"); 
			exit(EXIT_FAILURE);
		}

		if (semop(sem_d, sop1, 2) == -1) {
			perror("Semop fail");
			exit(EXIT_FAILURE);
		}

		if (shmctl(shm_d, SHM_UNLOCK, &sbuf) == -1) {
			perror("Shmctl fail"); 
			exit(EXIT_FAILURE);
		}

		shm_ptr = (char*)shmat(shm_d, 0, 0);

		if(shm_ptr == ((char*)-1)) {
			perror("Shmat fail");
			exit(EXIT_FAILURE);
		}

		p_operand1 = (double*)shm_ptr;
		p_opt = (char*)(p_operand1 + sizeof(double));
		p_operand2 = (double*)(p_opt + sizeof(char));

		*p_operand1 = stod(tmpd1);
		*p_opt = tmpc[0];
		*p_operand2 = stod(tmpd2);

		if(shmdt(shm_ptr) == -1) {
			perror("Shmdt fail(1)"); 
			exit(EXIT_FAILURE);
		}
		if (shmctl(shm_d, SHM_LOCK, &sbuf) == -1) {
			perror("Shmctl fail"); 
			exit(EXIT_FAILURE);
		}

		if (semop(sem_d, sop2, 2) == -1) {
			perror("Semop fail");
			exit(EXIT_FAILURE);
		}

		if (semop(sem_d, sop3, 2) == -1) { 
			perror("Semop fail"); 
			exit(EXIT_FAILURE);
		}
		if (shmctl(shm_d, SHM_UNLOCK, &sbuf) == -1) {
			perror("Shmctl fail"); 
			exit(EXIT_FAILURE);
		}
		
		shm_ptr = (char*)shmat(shm_d, 0, 0);
		if(shm_ptr == ((char*)-1)) {
			perror("Shmat fail"); 
			exit(EXIT_FAILURE);
		}

		p_operand1 = (double*)shm_ptr;
		cout << *p_operand1 << endl;

		if(shmdt(p_operand1) == -1) {
			perror("Shmdt fail(1)"); 
			exit(EXIT_FAILURE);
		}
		if (shmctl(shm_d, SHM_LOCK, &sbuf) == -1) {
			perror("Shmctl fail"); 
			exit(EXIT_FAILURE);
		}
		
		if (semop(sem_d, sop4, 2) == -1) {
			perror("Semop fail"); 
			exit(EXIT_FAILURE);
		}
	}
}