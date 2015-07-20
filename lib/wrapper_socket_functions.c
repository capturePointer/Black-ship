#include "net.h"

int Socket(int domain, int type, int protocol)
{
	int sockfd = socket(domain,type,protocol);

	if(sockfd == -1)
		prog_error("Socket error",true,errno);
	
	return sockfd;
}

int Connect(int socket, const struct sockaddr *address,socklen_t address_len)
{
	int connection = connect(socket, address, address_len);

	if(connection == -1)
		prog_error("Connect error",true,errno);
	
	return connection;
}
