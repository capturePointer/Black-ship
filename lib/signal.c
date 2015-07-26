#include "net.h"
/**
 * Wrapper signal functions
 */
Sigfunc *
Signal(int signo, Sigfunc *func)
{
	Sigfunc	*rsig;
	rsig = signal(signo, func);
	/*check if it fails*/
	if(rsig == SIG_ERR)
		prog_error("signal error",true,errno);
	
	//else return the signal
	return(rsig);
}
void
sig_h_child(int singo)
{
	pid_t 	pid;
	int 	stat;

	while( ( pid = waitpid(-1,&stat,WNOHANG)) > 0)
	{		
		/*for debugging reasons*/
		printf("child %d terminated\n",pid);
	}
}
