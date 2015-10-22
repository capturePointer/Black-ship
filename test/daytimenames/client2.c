#include "lib/sailfish.h"
int sockfd, n;
char recvline[MAXLINE + 1];
socklen_t len;
struct sockaddr_storage ss;


/**
 * ARGS MAP
 * argv[0] = ./client
 * argv[1] = hostname/ipaddr
 * argv[2] = service/port
 */
int 
main(int argc, char *argv[])
{
	if(argc != 3) {
		printf("Usage: client <hostname/ipaddress> <service/port#> ");
		exit(EXIT_FAILURE);
	}

	sockfd = establish_tcp(argv[1], argv[2]);
	len = sizeof(ss);
	
	Getpeername(sockfd, (SA *)&ss, &len);
	printf("connected ..");

	while( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		Fputs(recvline, stdout);
	}
	exit(EXIT_SUCCESS);

}
