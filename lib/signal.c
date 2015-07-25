#include "net.h"
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

	while(true)
	{
		pid = waitpid(-1,&stat,WNOHANG);
		if(pid <0) 
			prog_error("waipid error",true,errno);
		
		/*for debugging reasons*/
		printf("child %d terminated\n",pid);
	}
}
