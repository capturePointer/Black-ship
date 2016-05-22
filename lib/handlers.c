#include <unistd.h>

#include "handlers.h"
/**
 * Handler for singnal
 *  base functions
 */
// signum is the signal flag that will be pased to be handled
void
handler_child_wait(int signum)
{
	pid_t 	pid;
	int 	stat;

	pid = wait(&stat);

	if(pid < 0)
		 prog_error("Wait error",true,errno);
	// For debugging purpose
	// It is not recomanded tot use printf in signal handle func
	printf("Child terminated %d %d\n",pid,signum);
}
// Waitpid within a loop, fetching the status of any of
// the children that have terminated.We must specify the WNOHANG
// options.This tells waitpid not to block if there are running children
// that have not yet terminated.

// signum is the signal flag that will be pased to be handled
void
handler_child_waitpid(int signum)
{
	pid_t 	pid;
	int 	stat;
	while( ( pid = waitpid(WAIT_ANY,&stat,WNOHANG)) > 0) {
	// For debugging purpose
	// It is not recomanded tot use printf in signal handle func
		printf("child %d terminated %d \n",pid,signum);
	}
}
