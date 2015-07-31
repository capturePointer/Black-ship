#include "net.h"

void
Close(int fd)
{
	if(close(fd) == -1)
		prog_error("Clode error",true,errno);
}
pid_t
Fork(void)
{
	pid_t pid;
	if( (pid = fork()) <0 )
		prog_error("Fork error",true,errno);

	return pid;
}

pid_t
Waitpid(pid_t pid,int *ptr,int mode)
{
    pid_t rpid;
    rpid = waitpid(pid,ptr,mode);

    if(rpid == -1)
        prog_error("Waidpid error",true,errno);

    return rpid;
}

pid_t
Wait(int *status)
{
    pid_t rpid;

    rpid = wait(status);
    if(rpid == -1)
        prog_error("Wait error",true,errno);
    return rpid;
}
