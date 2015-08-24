#include "lib/net.h"
int sockfd;
void dg_cli(FILE* stream, int sockfd, const SA *server, socklen_t servlen);

int main(int argc, char *argv[])
{
	argc_length(argc,2);
	
	sockfd = Socket(AF_INET,SOCK_DGRAM,0);

	memset(&server4_address, 0, sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);

	dg_cli(stdin, sockfd, (SA*)&server4_address, sizeof(server4_address));

	exit(EXIT_SUCCESS);
}

void dg_cli(FILE *stream, int sockfd, const SA *server, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t len;
	struct sockaddr *preply_addr;

	preply_addr = Malloc(servlen);

	while(Fgets(sendline,MAXLINE,stream) != NULL)
	{
		Sendto(sockfd, sendline, strlen(sendline), 0, server, servlen);

		len = servlen;
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr,&len);
		if( len!=servlen || memcmp(server,preply_addr,len) !=0 )
		{
			printf("reply from %s (ignored) \n",proto_ntop(sockfd,preply_addr,len));
			continue;
		}

		recvline[n] = 0; /* null termiante*/
		Fputs(recvline, stdout);
	}
}

