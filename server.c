#include "lib/net.h"
int sockfd;
static void recvfrom_int(int);
static int count;
void dg_echo(int sockfd, SA *client, socklen_t clilen);

int main(void)
{
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);


	Bind(sockfd, (struct sockaddr *)&server4_address,sizeof(server4_address));

	dg_echo(sockfd, (SA*)&client4_address,sizeof(client4_address));

}


void dg_echo(int sockfd, SA *client, socklen_t clilen)
{
	socklen_t		len;
	char			mesg[MAXLINE];
	int				n;


	Signal(SIGINT, recvfrom_int);

	n = 220 * 1024;
	Setsockopt(sockfd, SOL_SOCKET,SO_RCVBUF, &n, sizeof(n));


	for(;;)
	{
		len = clilen;
		Recvfrom(sockfd, mesg, MAXLINE, 0,client, &len);
		count ++;
	}
}

static void
recvfrom_int(int signo)
{
	printf("\nrecived %d datagrams\n",count);
	exit(EXIT_SUCCESS);
}
