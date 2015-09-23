#include "seilfish.h"

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
Sendto(int fd, const void *buffer, size_t len_buffer, int flags,
	   const struct sockaddr  *client, socklen_t clilen)
{
	ssize_t nbytes_sended_to = sendto(fd,buffer,len_buffer,flags,client,clilen);
	if(nbytes_sended_to == -1)
		prog_error("Sendto error",true,errno);

	return len_buffer;
}
ssize_t
Recvfrom(int fd, void *restrict buffer, size_t len_buffer, int flags,
	     struct sockaddr *restrict client, socklen_t *restrict clilen)
{
	ssize_t nbytes_recived_from = recvfrom(fd,buffer,len_buffer,flags,
		                                   client,clilen);

	if(nbytes_recived_from == -1)
		prog_error("Recvfrom error",true,errno);

	return len_buffer;
}

int
Sctp_sendmsg(int s, const void *msg, size_t len, struct sockaddr *to,
            socklen_t tolen, uint32_t ppid, uint32_t flags,
            uint16_t stream_no, uint32_t timetolive,uint32_t context)

{
	int r = sctp_sendmsg(s,msg,len,to,tolen,ppid,flags,stream_no,
						 timetolive,context);
	if(r == -1)
		prog_error("Sctp_sendmsg error",true,errno);
	return r;
}

int
Sctp_recvmsg(int s, void *msg, size_t len, struct sockaddr *from,
			 socklen_t *fromlen, struct sctp_sndrcvinfo *sinfo,int *msg_flags)

{
	int r = sctp_recvmsg(s,msg,len,from,fromlen,sinfo,msg_flags);
	if(r == -1)
		prog_error("Sctp_recvmsg error",true,errno);
	return r;
}
/**
 * Sctp_send is a wrapper library function 
 * that can be used to send a message from a socket without 
 * the use of the CMSG header structures. sd is the socket descriptor
 * from which the message pointed to by msg of length len is sent. sinfo
 * is a pointer to a sctp_sndrcvinfo structure. flags parameter is composed
 * of a bitwise OR of the flags that can be be passed as the 3rd argument of
 * a standard sendmsg() call.
 *
 */
int 
Sctp_send(int s, const void *msg, size_t len,
			  const struct sctp_sndrcvinfo *sinfo ,uint32_t flags)
{
	 int r = sctp_send(s, msg, len, sinfo, flags);
	 if( r == -1)
	 	 prog_error("Sctp_send error", true, errno);
	 return r;

}

ssize_t 
Send(int fd, const void *buffer, size_t len_buffer, int flags)
{
    ssize_t r = Send(fd, buffer, len_buffer, flags);
    if(r == -1)
    	prog_error("Send error", true, errno);
    return r;
}

ssize_t
Recv(int fd, void *buffer, size_t len_buffer, int flags)
{
	 ssize_t r = Recv(fd, buffer, len_buffer, flags);
	 if(r == -1)
	 	 prog_error("Recv error", true, errno);
	 return r;
}

ssize_t
Recvmsg(int fd, struct msghdr *message, int flags)
{
	 ssize_t r = recvmsg(fd, message, flags);
	 if(r == -1)
	 	 prog_error("Recvmsg error", true, errno);
	 return r;
}
ssize_t
Sendmsg(int fd, const struct msghdr *message,int flags)
{
	 ssize_t r = sendmsg(fd, message, flags);
	 if(r == -1)
	 	 prog_error("Sendmsg error", true, errno);
	 return r;
}
