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

/*Presentation to network byte order*/
int Inet_pton(int af,const char *src, void *dst)
{

	int inet = inet_pton(af,src,dst);

	if(inet == -1)
	{
		printf("Error : %s\n",strerror(errno));
		return -1;	
	}
	else 
		if(inet == 0)
		{
			printf("Error: Ip you provided does not contain a valid representation of a valid network address \n");
			return 0;
		}

	return inet;

}

