#include "net.h"


size_t Write(int fd,const void *buffer,size_t length)
{
	size_t nbytes;
	size_t false_bytes = -1;
	nbytes = write(fd,buffer,length);

	if(nbytes < false_bytes)
		error_system_exit("Can't write to the socket descriptor \n",false_bytes);
	
	return nbytes;
		
}
