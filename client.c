#include "lib/net.h"
int sockfd;
void dg_cli(FILE* stream, int sockfd, const SA *server, socklen_t servlen);

int main(int argc, char *argv[])
{
	argc_length(argc,2);
	
	sockfd = Socket(AF_INET6,SOCK_DGRAM,0);

	memset(&server6_address, 0, sizeof(server6_address));

	server6_address.sin6_family = AF_INET6;
	server6_address.sin6_port = htons(PORT);
	Inet_pton(AF_INET6, argv[1], &server6_address.sin6_addr);

	dg_cli(stdin, sockfd, (SA*)&server6_address, sizeof(server6_address));

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
//		if( len!=servlen || memcmp(server,preply_addr,len) !=0 )
//		{
			printf("%s\n",proto_ntop(sockfd,preply_addr,len));
//			continue;
//		}

		recvline[n] = 0; /* null termiante*/
		Fputs(recvline, stdout);
	}
}

