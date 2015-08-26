#include "lib/net.h"
int sockfd;

void dg_echo(int sockfd, SA *client, socklen_t clilen);

int main(void)
{
	sockfd = Socket(AF_INET6, SOCK_DGRAM, 0);

	memset(&server6_address,0,sizeof(server4_address));

	server6_address.sin6_family = AF_INET6;
	server6_address.sin6_addr = in6addr_any;
	server6_address.sin6_port = htons(PORT);


	Bind(sockfd, (SA*)&server6_address,sizeof(server6_address));

	dg_echo(sockfd, (SA*)&client6_address,sizeof(client6_address));

}
void dg_echo(int sockfd, SA *client, socklen_t clilen)
{
	int n;
	socklen_t len;
	char sendbuffer[MAXLINE];

	for(; ;)
	{
		len = clilen;
		n = Recvfrom(sockfd, sendbuffer, MAXLINE, 0,client,&len);

		Sendto(sockfd, sendbuffer, n, 0,client, len);
	}
}
