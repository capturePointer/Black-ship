#include "pidwait.h"

int sockfd;
int i;

void 
sig_pipe(int signo){
	printf("SIGPIPE RECIVED %d\n",signo);
	return;
}


int
main(int argc, char *argv[])
{

		argc_length(argc, 2);
		sockfd = Socket(AF_INET, SOCK_STREAM, 0);
		memset(&server4_address, 0 ,sizeof(server4_address));
		server4_address.sin_port = htons(PORT);
		server4_address.sin_family = AF_INET;
		Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);
		Signal(SIGPIPE, sig_pipe);
		Connect(sockfd, (SA*)&server4_address, sizeof(server4_address));
		str_cli(stdin, sockfd);
		exit(EXIT_SUCCESS);
}
// This example generates a sigpipe
void str_cli(FILE*stream,int sock)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	//sendline it holds \n
	while((Fgets(sendline,MAXLINE,stream) != NULL)) {
		// All we have changed to call s_write(safe_write) two times
		// the first time the first byte of data is written to the socket
		s_write(sock,sendline,1,false);
		//followed by a pause of one second
		sleep(1);
		// followed by the reminder of the line
		s_write(sock, sendline+1, strlen(sendline)-1, false);

		if(readline(sock, recvline, MAXLINE) == 0) {
			fprintf(stderr, "%s", "Server fuck it up\n");
			exit(EXIT_FAILURE);
		}
		Fputs(recvline, stdout);
	}
}
