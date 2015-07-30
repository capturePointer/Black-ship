#include "net.h"

int Socket(int domain, int type, int protocol)
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
