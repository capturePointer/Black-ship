#include "lib/net.h"
int sockfd;

void dg_echo(int sockfd, SA *client, socklen_t clilen);

int main(void)
{
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);


	Bind(sockfd, (SA*)&server4_address,sizeof(server4_address));

	dg_echo(sockfd, (SA*)&client4_address,sizeof(client4_address));

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
