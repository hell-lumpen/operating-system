#include "wrappers.h"
#include "bigint.h"

int main () {
	key_t sem_key = ftok("server_on_shared_memory.cpp", 0);
	key_t shm_key = ftok("server_on_shared_memory.cpp", 10);

	int sem_id = semget(sem_key, 2, IPC_CREAT | IPC_EXCL | 0666);
	if (sem_id == -1) {
		if (errno == EEXIST) {
			sem_id = semget(sem_key, 0, 0);
			if (sem_id == -1) perror("semget error");
		}
		else
			perror("semget error");
	}
	else change_semaphores(sem_id, RESET_SEM);


	int shm_id = shmget(shm_key, MSG_SIZE, 
		IPC_CREAT | IPC_EXCL | 0666);
	if (shm_id == -1) {
		if (errno == EEXIST) {
			shm_id = shmget(shm_key, 0, 0);
			if (shm_id == -1) perror("shmget error");
		}
		else
			perror("shmget error");
	}

	printf("The server on shared memory is running\n\n");

	char* shared_memory_ptr = (char*) shmat(shm_id, NULL, 0);
	
	if (shared_memory_ptr  == (char*) -1) {
		perror("shmat");
	}

	while (true) 
	{

	char *msg = (char* )malloc(MSG_SIZE * sizeof(char));

	recv_msg(shared_memory_ptr, msg, sem_id, true);
	printf("recived: %s\n", msg + sizeof(char));

	char operator_ = ' ';
    size_t index = 0;

    std::string test = std::string(msg + sizeof(char));
    free(msg);

    for (size_t i = 0; i < test.length(); i++) {
    	if (test[i] == '+' || test[i] == '-') {
    		operator_ = test[i];
    		index = i;
    		break;
    	}
    }

    std::string n1, n2;

    if (test.empty()) {
        send_msg(shared_memory_ptr, "ERROR:\t"
        	"Expression is empty", sem_id, true);
        printf("sended: Expression is empty\n\n");
        continue;
    }
    else if (operator_ == '+' && index != 0) {
    	n1 = test.substr(0, index);
    	n2 = test.substr(index + 1);
        send_msg(shared_memory_ptr, sum(10, n1, n2).c_str(), 
        	sem_id, true);
        std::cout << "1" << std::endl;
        continue;
    }
    else if (operator_ == '-' && index != 0) {
    	n1 = test.substr(0, index);
    	n2 = test.substr(index + 1);
        send_msg(shared_memory_ptr, minus(10, n1, n2).c_str(), 
        	sem_id, true);
        std::cout << "2" << std::endl;
        continue;
    }

    send_msg(shared_memory_ptr, "ERROR:\t"
    	"Couldn't interpret the expression", sem_id, true);
    printf("sended: Couldn't interpret the expression\n\n");

	}

	if (semctl(sem_id, 0, IPC_RMID, 0) == -1) {
		perror("semctl (IPC_RMID)");
	}
	if (shmctl(shm_id, IPC_RMID, 0) == -1) {
		perror("shmctl (IPC_RMID)");
	}
	return 0;
}
