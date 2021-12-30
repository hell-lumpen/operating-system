#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
using namespace std;

typedef struct {
	double real;
	double imaginary;
} number_representation;

typedef struct message_buffer {
	long mtype;
	int childpid;
	int ratio[4];
	number_representation ans[3];
} message_buffer;

double mypow(double x){
	if (x < 0) return -pow(-x, 1.0/3.0);
	return pow(x, 1.0/3.0);
}

int Kardano(double A, double B, double C, double D, number_representation* x){
	if (A == 0) return -1;

	double p = (3.0*A*C-B*B)/(3.0*A*A);
	double q = (2.0*B*B*B-9.0*A*B*C+27.0*A*A*D)/(27.0*A*A*A);
	double S = (q*q/4.0) + (p*p*p/27.0);
	double F;

	if (q == 0)
		F = M_PI/2.0;
	if (q < 0)
		F = atan(-2.0*sqrt(-S)/q);
	if (q > 0)
		F = atan(-2.0*sqrt(-S)/q) + M_PI;

	for (int i = 0; i < 3; i++)
		x[i].real = x[i].imaginary = 0;

	if (S < 0){
		x[0].real = 2.0*sqrt(-p/3.0)*cos(F/3.0)-B/(3.0*A);
		x[1].real = 2.0*sqrt(-p/3.0)*cos((F/3.0)+2.0*M_PI/3.0)-B/(3.0*A);
		x[2].real = 2.0*sqrt(-p/3.0)*cos((F/3.0)+4.0*M_PI/3.0)-B/(3.0*A);
	}

	if (S == 0){
		x[0].real = 2.0*mypow(-q/2.0)-B/(3.0*A);
		x[1].real = -mypow(-q/2.0)-B/(3.0*A);
		x[2].real = -mypow(-q/2.0)-B/(3.0*A);
	}

	if (S > 0){
		double temp1 = mypow((-q/2.0)+sqrt(S)) + mypow((-q/2.0)-sqrt(S));
		double temp2 = mypow((-q/2.0)+sqrt(S)) - mypow((-q/2.0)-sqrt(S));
		x[0].real = temp1 - B/(3.0*A);
		x[1].real = -temp1/2.0 - B/(3.0*A); x[1].imaginary = sqrt(3)*temp2/2.0;
		x[2].real = -temp1/2.0 - B/(3.0*A); x[2].imaginary = -sqrt(3)*temp2/2.0;
	}
	return 0;
}

int main () {
	message_buffer mbuf;
	
	number_representation x[3];
	int pid = getpid();

	int fd = msgget(10, IPC_CREAT| 0666);
	if (fd == -1) perror("msgget");

	while (true) {
		if (msgrcv(fd,&mbuf, 1024, 1 , 0) > 0) {

			if (Kardano(mbuf.ratio[0],mbuf.ratio[1],mbuf.ratio[2],mbuf.ratio[3], x) == -1){
				printf("coefficient _a_ cannot be zero\n");
				break;
			}

			mbuf.ans[0] = x[0];
			mbuf.ans[1] = x[1];
			mbuf.ans[2] = x[2];

			mbuf.mtype = mbuf.childpid;
			msgsnd(fd, &mbuf, sizeof(mbuf.ans)*2, 0);
		}
	}

	msgctl(fd, IPC_RMID, 0);
	return 0;
}