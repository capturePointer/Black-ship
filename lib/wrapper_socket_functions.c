#include "sailfish.h"

int
Socket(int domain, int type, int protocol)
{
	int sockfd = socket(domain,type,protocol);
	if(sockfd == -1)
		prog_error("Socket error",true,errno);
	return sockfd;
}

void
Connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	int connection = connect(socket, address, address_len);
	if(connection < 0)
		prog_error("Connect error",true,errno);
}

void
Bind(int socket,const struct sockaddr *address,socklen_t address_len)
{
	int rbind = bind(socket,address ,address_len);
	if(rbind == -1)
		prog_error("Bind error",true,errno);
}
void
Listen(int socket, int backlog)
{
	int rlisten = listen(socket,backlog);
	if(rlisten == -1)
		prog_error("Listen error",true,errno);
}
int
Accept(int socket, struct sockaddr  *restrict address, socklen_t  *restrict address_len)
{
	int raccept;
	raccept = accept(socket,address,address_len);
	if(raccept == -1)
		prog_error("Accept error",true,errno);

	return raccept;
}
int
Select(int nfd,fd_set *readfd, fd_set *writefd, fd_set *exceptfd, struct timeval *timeout)
{
    int number_of_ready_fds;
    if( (number_of_ready_fds = select(nfd, readfd, writefd, exceptfd, timeout)) < 0)
        prog_error("Select error",true,errno);
    return number_of_ready_fds;
}
void
Shutdown(int fd, int mode)
{
    int rshutdown;
    rshutdown = shutdown(fd,mode);
    if(rshutdown < 0)
        prog_error("Shutdown error",true,errno);
}
int
Poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
	int rpoll;
	rpoll = poll(fds,nfds,timeout);
	if(rpoll == -1)
		prog_error("Poll error",true,errno);

	return rpoll;
}
void
Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
	if(getsockopt(fd, level, optname, optval, optlenptr) == -1)
		prog_error("Getsockopt error",true,errno);
}
void
Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
	if(setsockopt(fd, level, optname, optval, optlen) == -1)
		prog_error("Setsockoopt error",true,errno);
}
void
Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if(getpeername(fd, sa, salenptr) == -1)
		prog_error("Getpeername error",true,errno);
}
void
Getsockname(int fd, struct sockaddr *restrict sa, socklen_t *restrict len)
{
	if(getsockname(fd,sa,len) == -1 )
		prog_error("Getsockname error",true,errno);
}
/*SCTP WRAPPER */
void
Sctp_bindx(int sd, struct sockaddr *addrs, int addrcnt, int flags)
{
	int rbindx = sctp_bindx(sd, addrs, addrcnt, flags);
	if(rbindx == -1)
		prog_error("Sctp_bindx error",true,errno);
}
void
Sctp_connectx(int sd, struct sockaddr *addrs, int addrcnt, sctp_assoc_t *id)
{
	int rconnectx = sctp_connectx(sd, addrs, addrcnt, id);
	if(rconnectx == -1)
		prog_error("Sctp_connectx error",true,errno);
}
int
Sctp_peeloff(int sd, sctp_assoc_t assoc_id)
{
	int rpeeloff = sctp_peeloff(sd, assoc_id);
	if(rpeeloff == -1)
		prog_error("Sctp_paleof error",true,errno);
	return rpeeloff;
}
void
Sctp_getpaddrs(int sd, sctp_assoc_t id, struct sockaddr **addrs)
{
	int rgetpaddrs = sctp_getpaddrs(sd, id, addrs);
	if(rgetpaddrs == -1)
		prog_error("Sctp_getpaddrs error",true,errno);
}
void
Sctp_freepaddrs(struct sockaddr *addrs)
{
	sctp_freepaddrs(addrs);
}
void
Sctp_getladdrs(int sd, sctp_assoc_t id, struct sockaddr **addrs)
{
	int rgetladdrs = sctp_getladdrs(sd, id, addrs);
	if(rgetladdrs == -1)
		prog_error("Sctp_getladdrs error",true,errno);
}
void
Sctp_freeladdrs(struct sockaddr *addrs)
{
	sctp_freeladdrs(addrs);
}

void
Sctp_opt_info(int sd, sctp_assoc_t id, int opt, void *arg, socklen_t *size)
{
	int ropt = sctp_opt_info(sd, id, opt, arg, size);
	if(ropt == -1)
		prog_error("Sctp_opt_info error",true,errno);
}

