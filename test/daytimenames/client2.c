#include "lib/seilfish.h"

int sockfd, n;
char recvline[MAXLINE+1];
struct sockaddr_storage ss;
socklen_t len;

int 
main(int argc, char **argv) 
{
	/**
	 * We now require a second  command-line argument to specify either the
	 * service name or the port number, which allows our program to connect
	 * to other ports
	 */
	argc_length(argc,3);
	 // return a valid tcp socket
	sockfd = establish_tcp(argv[1], argv[2]);

	len = (socklen_t)sizeof(ss);
    // get name of the connection socket
	/**
	 * We call Getpeername to fetch the server's protocol addr
	 * and print it. We do this to verify the protocol being
	 * usied in the examples..
	 * Note that tcp_connect does not return the size of the socket addr
	 * structure that was used goal for this function was to reduce the 
	 * number of arguments compared to getaddrinfo.What we do instead is use a
	 * socketstorage socket addresss structure, which is large enogh to hold and fulfills the
	 * alignment constraints of any socket addr type the system supports
	 * 
	 */
	
	Getpeername(sockfd, (SA*)&ss, &len);
	//printf("connected to %s\n", Sock_ntop_host((SA *) &ss, len));

	while( (n = Read(sockfd, recvline, MAXLINE)) > 0 ){
		recvline[n] = '\0';
		Fputs(recvline, stdout);
	}

	exit(EXIT_SUCCESS);

}
