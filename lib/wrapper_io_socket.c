#include "net.h"

/*Wrapper write*/
size_t 
Write(int fd, const void *buffer,size_t len_buffer)
{
	ssize_t nbytes_writen = write(fd,buffer,len_buffer);

	if(nbytes_writen == -1)
		prog_error("Write error",true,errno);

	return len_buffer;
}

size_t 
Read(int fd, void *buffer, size_t len_buffer)
{
	ssize_t nbytes_readed = read(fd,buffer,len_buffer);

	if(nbytes_readed == -1)
		prog_error("Read error",true,errno);
	
	return len_buffer;
}
