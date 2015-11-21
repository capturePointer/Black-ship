#include "pidwait.h"

int sockfd;

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
	server4_address.sin_port = htons(13);
	server4_address.sin_family = AF_INET;
	
	Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);
	
	Signal(SIGPIPE, sig_pipe);
	
	Connect(sockfd, (SA*)&server4_address, sizeof(server4_address));

	sleep(2);
	Write(sockfd, "hello",5);
	sleep(2);
	Write(sockfd, "world",5);

	exit(EXIT_SUCCESS);
}

