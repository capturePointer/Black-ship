#include "sailfish.h"
/**
 * Handler for singnal
 *  base functions
 */
void
handler_child_wait(int signo)
{
	pid_t 	pid;
	int 	stat;

	pid = wait(&stat);

	if(pid < 0)
		 prog_error("Wait error",true,errno);

	printf("Child terminated %d\n",pid);
}

void
handler_child_waitpid(int singo)
{
	pid_t 	pid;
	int 	stat;

	while( ( pid = waitpid(-1,&stat,WNOHANG)) > 0)
	{
		/*for debugging reasons*/
		printf("child %d terminated\n",pid);
	}
}
