
#include "lib/sailfish.h"
void 
str_cli(FILE *stream, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	while(Fgets(sendline,MAXLINE, stream) != NULL) {
		s_write(sockfd, sendline, strlen(sendline), true);

		if(readline(sockfd, recvline, MAXLINE) == 0) {
			prog_error("server exit prematurely\n",true, errno);
		}

		Fputs(recvline, stdout);
	}
}

int 
main(int argc, char **argv)
{
	argc_length(argc, 2);
	
	int listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&server4_address, 0, sizeof(server4_address));
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);
	
	Connect(listenfd, (SA* )&server4_address, sizeof(server4_address));
	
	str_cli(stdin, listenfd);

	exit(EXIT_SUCCESS);
}
