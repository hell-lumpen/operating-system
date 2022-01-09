#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

using namespace std;

typedef struct {
	double real;
	double imaginary;
} number_representation;

typedef struct message_buffer {
	long mtype;
	int childpid;
	int ratio[4]= {0,0,0,0};
	number_representation ans[3];
} message_buffer;

int main () {
	int mypid = getpid();
	message_buffer mbuf;
	int fd = msgget (10, IPC_CREAT | 0666);

	while (true) {
		
	cout << "Enter ratio of cubic equation "
	"(A,B,C,D: Ax + By + Cz + D = 0)" << endl;
	cin >> mbuf.ratio[0] >> mbuf.ratio[1] 
	>> mbuf.ratio[2] >> mbuf.ratio[3];
	
	mbuf.childpid = mypid;
	mbuf.mtype = 1;
	size_t len = sizeof(mbuf.ans);
	ssize_t msgsnd_res = msgsnd(fd, &mbuf, 1024, 0);
	if (msgsnd_res == -1) perror("msgsnd: ");

	if (msgrcv(fd, &mbuf, 1024, mypid , 0) > 0) {
		
	for(int i = 0; i < 3; i++) {
		if (mbuf.ans[i].imaginary == 0.0)
			printf("x%d = %.15lf\n", i, mbuf.ans[i].real);
		else
			printf("x%d = %.15lf + i * %.15lf\n",
			 i, mbuf.ans[i].real, mbuf.ans[i].imaginary);
		}
	}
	break;

	}
	return 0;
}