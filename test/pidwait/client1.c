#include "pidwait.h"

int sockfd[5];
int i;

int
main(int argc, char *argv[])
{
	for(i = 0; i<5; i++) {

		argc_length(argc, 2);
		sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);
		memset(&server4_address, 0 ,sizeof(server4_address));
		server4_address.sin_port = htons(PORT);
		server4_address.sin_family = AF_INET;
		Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);
		Connect(sockfd[i], (SA*)&server4_address, sizeof(server4_address));
		fprintf(stdout,"Connect socket %d\n", i);
	}

	str_cli(stdin,sockfd[0]);
	exit(EXIT_SUCCESS);
}

void str_cli(FILE*stream,int sock)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	//sendline it holds \n
	while((Fgets(sendline,MAXLINE,stream) != NULL)) {
		s_write(sock,sendline,strlen(sendline),false);

		if(readline(sock, recvline, MAXLINE) == 0) {

			fprintf(stderr, "%s", "Server fuck it up\n");

			exit(EXIT_FAILURE);
		}

		Fputs(recvline, stdout);
	}
}
