#ifndef PIDWAIT_H
#define PIDWAIT_H

#include "../../lib/sailfish.h"

void str_cli(FILE*,int);
void str_echo(int);
void sig_pipe(int);
void sig_int(int);

// sum
struct args {
	long arg1;
	long arg2;

};

struct result {
	long sum;
};
#endif
