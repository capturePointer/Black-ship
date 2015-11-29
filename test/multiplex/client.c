#include "multiplex.h"

const char *ipaddr = "127.0.0.1";

int sockfd;
int
main(void)
{ 
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&server4_address, 0 , sizeof(server4_address));

	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	Inet_pton(AF_INET, ipaddr,&server4_address.sin_addr);

	Connect(sockfd, (SA*)&server4_address, sizeof(server4_address));
	
	str_cli(stdin, sockfd);

	exit(EXIT_SUCCESS);
}

void
str_cli(FILE *stream,int sock)
{
	char sendline[MAXLINE], recvline[MAXLINE];

	while(Fgets(sendline,MAXLINE,stream) != NULL) {

		s_write(sock, sendline, strlen(sendline), true);

		if(readline(sock, recvline,MAXLINE) == 0) {
			printf("server termianted\n");
		}

		Fputs(recvline, stdout);
	}
}

