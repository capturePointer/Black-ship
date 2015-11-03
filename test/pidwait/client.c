#include "lib/sailfish.h"
int i, sockfd[5];

void
str_cli(FILE *stream, int sockfd)
 {
	char sendline[MAXLINE], recvline[MAXLINE];

	while( Fgets(sendline, MAXLINE, stream) != NULL) {
		int n = strlen(sendline);
		sendline[n-1] = '\0';
		s_write(sockfd, sendline, strlen(sendline), true);
		if(readline(sockfd, recvline, MAXLINE) == 0) {
			printf("Server terminated prematurerly\n");
			exit(EXIT_FAILURE);
		}
		Fputs(recvline, stdout);
	}
}

int
main(int argc, char **argv)
{
		argc_length(argc,2);
		// loop 5 times and create 5 sockets and 5 connections
		for(i=0; i<5; i++){
			sockfd[i] = Socket(AF_INET,SOCK_STREAM,0);

			memset(&server4_address, 0, sizeof(server4_address));

			server4_address.sin_family = AF_INET;
			server4_address.sin_port = htons(PORT);
			Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);

			Connect(sockfd[i],(SA*)&server4_address,sizeof(server4_address));
		}

	str_cli(stdin, sockfd[0]);
// When the client terminates,al open descriptor are closed automatically by
// the kernel 	(we do not call close, only exit),and all five connections
// are terminated at about the same time.This causes the FINs o be sent, one
// on each time.This causes five SIGCHLD signals to be delivered to the
// parent at about the same time.
	exit(EXIT_SUCCESS);
}