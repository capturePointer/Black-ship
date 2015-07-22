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
