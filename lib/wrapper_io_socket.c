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
ssize_t
Sendto(int fd, const void *buffer, size_t len_buffer, int flags, const struct sockaddr  *client, socklen_t clilen)
{
	ssize_t nbytes_sended_to = sendto(fd,buffer,len_buffer,flags,client,clilen);
	if(nbytes_sended_to == -1)
		prog_error("Sendto error",true,errno);

	return len_buffer;
}
ssize_t
Recvfrom(int fd, void *restrict buffer, size_t len_buffer, int flags, struct sockaddr *restrict client, socklen_t *restrict clilen)
{
	ssize_t nbytes_recived_from = recvfrom(fd,buffer,len_buffer,flags,client,clilen);
	if(nbytes_recived_from == -1)
		prog_error("Recvfrom error",true,errno);

	return len_buffer;
}
