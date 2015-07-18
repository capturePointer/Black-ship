#include "net.h"

int Socket(int domain, int type, int protocol)
{

	int sockfd = socket(domain,type,protocol);

	if(sockfd == -1)
	{
		printf("Error socket: %s\n",strerror(errno));
		return -1;
	}

	return sockfd;
		
}

int Connect(int socket, const struct sockaddr *address,socklen_t address_len)
{
	int connection = connect(socket, address, address_len);

	if(connection == -1)
	{
		printf("Error: %s\n",strerror(errno));
		return -1;
	}
	return connection;
}
