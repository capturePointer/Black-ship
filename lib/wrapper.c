#include "wrapper.h"

/*
 * Presentation to network byte order
 */
void sailfish_pton(int af, const char *src, void *dst)
{
	int inet = inet_pton(af,src,dst);

	if(inet == -1)
		prog_error("Ip conversion error",true,errno);
	else
		if(inet == 0)
			prog_error("Invalid ip address",false,errno);
}

/**
 * Network to presentation
 */
const char *sailfish_ntop(int af, const void *src, char *dst, socklen_t size)
{
	const char *inet = inet_ntop(af,src,dst,size);
	if(inet == NULL)
		prog_error("Network to presentaion converison error",true,errno);
	return inet;
}




/**
 * Convert Ip adress form dots and number string to a struct in_addr and back
 */

// NOTE THAT THIS METHOD IS DEPRECATED ! Use inet_pton() or inet_ntop instead

/**
 * This functions are deprecated because they don't handle IPV6 ! They are included here
 * because they can still be found in the wild
 */

void sailfish_aton(const char *cp, struct in_addr *inp){
   int n;
   if( (n = inet_aton(cp, inp)) == 0){
		prog_error("Inet_aton invalid address", true, errno);
   }
}

/*Wrapper write*/
ssize_t sailfish_write(int fd, const void *buffer,size_t len_buffer)
{
	ssize_t nbytes_writen = write(fd,buffer,len_buffer);

	if(nbytes_writen == -1)
		prog_error("Write error",true,errno);

	return (ssize_t)len_buffer;
}

ssize_t sailfish_read(int fd, void *buffer, size_t len_buffer)
{
	ssize_t nbytes_readed = read(fd,buffer,len_buffer);

	if(nbytes_readed == -1)
		prog_error("Read error",true,errno);

	return (ssize_t)len_buffer;
}

ssize_t sailfish_sendto(int fd, const void *buffer, size_t len_buffer, int flags,
	   const struct sockaddr  *client, socklen_t clilen)
{
	ssize_t nbytes_sended_to = sendto(fd,buffer,len_buffer,flags,client,clilen);
	if(nbytes_sended_to == -1)
		prog_error("Sendto error",true,errno);

	return (ssize_t)len_buffer;
}

ssize_t sailfish_recvfrom(int fd, void *restrict buffer, size_t len_buffer, int flags,
						struct sockaddr *restrict client, socklen_t *restrict clilen)
{
	ssize_t nbytes_recived_from = recvfrom(fd,buffer,len_buffer,flags,
		                                   client,clilen);

	if(nbytes_recived_from == -1)
		prog_error("Recvfrom error",true,errno);

	return (ssize_t)len_buffer;
}

int sailfish_sctp_sendmsg(int s, const void *msg, size_t len, struct sockaddr *to,
						socklen_t tolen, uint32_t ppid, uint32_t flags,
						uint16_t stream_no, uint32_t timetolive,uint32_t context)

{
	int r = sctp_sendmsg(s,msg,len,to,tolen,ppid,flags,stream_no,
						 timetolive,context);
	if(r == -1)
		prog_error("Sctp_sendmsg error",true,errno);
	return r;
}

int sailfish_sctp_recvmsg(int s, void *msg, size_t len, struct sockaddr *from,
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

int sailfish_sctp_send(int s, const void *msg, size_t len,
			  const struct sctp_sndrcvinfo *sinfo ,int flags)
{
	 int r = sctp_send(s, msg, len, sinfo, flags);
	 if( r == -1)
	 	 prog_error("Sctp_send error", true, errno);
	 return r;

}

ssize_t sailfish_send(int fd, const void *buffer, size_t len_buffer, int flags)
{
    ssize_t r = send(fd, buffer, len_buffer, flags);
    if(r == -1)
    	prog_error("Send error", true, errno);
    return r;
}

ssize_t sailfish_recv(int fd, void *buffer, size_t len_buffer, int flags)
{
	 ssize_t r = recv(fd, buffer, len_buffer, flags);
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
sailfish_sendmsg(int fd, const struct msghdr *message,int flags)
{
	 ssize_t r = sendmsg(fd, message, flags);
	 if(r == -1)
	 	 prog_error("Sendmsg error", true, errno);
	 return r;
}


int
sailfish_socket(int domain, int type, int protocol)
{
	int sockfd = socket(domain,type,protocol);
	if(sockfd == -1)
		prog_error("Socket error",true,errno);
	return sockfd;
}

void sailfish_connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	int connection = connect(socket, address, address_len);
	if(connection < 0)
		prog_error("Connect error",true,errno);
}

void sailfish_bind(int socket,const struct sockaddr *address,socklen_t address_len)
{
	int rbind = bind(socket,address ,address_len);
	if(rbind == -1)
		prog_error("Bind error",true,errno);
}
void sailfish_listen(int socket, int backlog)
{
	int rlisten = listen(socket,backlog);
	if(rlisten == -1)
		prog_error("Listen error",true,errno);
}
int sailfish_accept(int socket, struct sockaddr  *restrict address, socklen_t  *restrict address_len)
{
	int raccept;
	raccept = accept(socket,address,address_len);
	if(raccept == -1)
		prog_error("Accept error",true,errno);

	return raccept;
}
int sailfish_accept(int nfd,fd_set *readfd, fd_set *writefd, fd_set *exceptfd, struct timeval *timeout)
{
    int number_of_ready_fds;
    if( (number_of_ready_fds = select(nfd, readfd, writefd, exceptfd, timeout)) < 0)
        prog_error("Select error",true,errno);
    return number_of_ready_fds;
}
void sailfish_shutdown(int fd, int mode)
{
    int rshutdown;
    rshutdown = shutdown(fd,mode);
    if(rshutdown < 0)
        prog_error("Shutdown error",true,errno);
}

int sailfish_poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
	int rpoll;
	rpoll = poll(fds,nfds,timeout);
	if(rpoll == -1)
		prog_error("Poll error",true,errno);

	return rpoll;
}
void sailfish_getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
	if(getsockopt(fd, level, optname, optval, optlenptr) == -1)
		prog_error("Getsockopt error",true,errno);
}

void sailfish_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
	if(setsockopt(fd, level, optname, optval, optlen) == -1)
		prog_error("Setsockoopt error",true,errno);
}
void sailfish_getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if(getpeername(fd, sa, salenptr) == -1)
		prog_error("Getpeername error",true,errno);
}
void sailfish_getsockname(int fd, struct sockaddr *restrict sa, socklen_t *restrict len)
{
	if(getsockname(fd,sa,len) == -1 )
		prog_error("Getsockname error",true,errno);
}
/*SCTP WRAPPER */
void sailfish_sctp_bindx(int sd, struct sockaddr *addrs, int addrcnt, int flags)
{
	int rbindx = sctp_bindx(sd, addrs, addrcnt, flags);
	if(rbindx == -1)
		prog_error("Sctp_bindx error",true,errno);
}
void sailfish_sctp_connectx(int sd, struct sockaddr *addrs, int addrcnt, sctp_assoc_t *id)
{
	int rconnectx = sctp_connectx(sd, addrs, addrcnt, id);
	if(rconnectx == -1)
		prog_error("Sctp_connectx error",true,errno);
}
int sailfish_sctp_peeloff(int sd, sctp_assoc_t assoc_id)
{
	int rpeeloff = sctp_peeloff(sd, assoc_id);
	if(rpeeloff == -1)
		prog_error("Sctp_paleof error",true,errno);
	return rpeeloff;
}
void sctp_getpaddrs(int sd, sctp_assoc_t id, struct sockaddr **addrs)
{
	int rgetpaddrs = sctp_getpaddrs(sd, id, addrs);
	if(rgetpaddrs == -1)
		prog_error("Sctp_getpaddrs error",true,errno);
}
void sctp_freepaddrs(struct sockaddr *addrs)
{
	sctp_freepaddrs(addrs);
}
void sctp_getladdrs(int sd, sctp_assoc_t id, struct sockaddr **addrs)
{
	int rgetladdrs = sctp_getladdrs(sd, id, addrs);
	if(rgetladdrs == -1)
		prog_error("Sctp_getladdrs error",true,errno);
}
void sailfish_sctp_freeladdrs(struct sockaddr *addrs)
{
	sctp_freeladdrs(addrs);
}

void sctp_opt_info(int sd, sctp_assoc_t id, int opt, void *arg, socklen_t *size)
{
	int ropt = sctp_opt_info(sd, id, opt, arg, size);
	if(ropt == -1)
		prog_error("Sctp_opt_info error",true,errno);
}
/*
 * Our own standard I/O wrapper functions
 */
char *sailfish_fgets(char *pointer,int n, FILE *stream)
{
	char *fpointer = fgets(pointer,n,stream);
	int file_error = ferror(stream);

	// if fgets returned null and ferror return true
	if( (fpointer == NULL) && (file_error) ) {
        echo_error("Can't read line from stdin",true,errno);
		return NULL;
	}
	return fpointer;
}

void sailfish_fputs(const char *pointer, FILE *stream)
{
	if(fputs(pointer,stream) == EOF )
	 	prog_error("Can't output line from buffer given",true,errno);
}

void sailfish_close(int fd)
{
	if(close(fd) == -1)
		prog_error("Clode error",true,errno);
}

pid_t sailfish_fork(void)
{
	pid_t pid;
	if((pid = fork()) < 0)
		prog_error("Fork error",true,errno);
	return pid;
}

pid_t sailfish_waitpid(pid_t pid,int *ptr,int mode)
{
    pid_t rpid;
    rpid = waitpid(pid,ptr,mode);

    if(rpid == -1)
        prog_error("Waidpid error",true,errno);
    return rpid;
}

pid_t sailfish_wait(int *status)
{
    pid_t rpid;

    rpid = wait(status);
    if(rpid == -1)
        prog_error("Wait error",true,errno);
    return rpid;
}
