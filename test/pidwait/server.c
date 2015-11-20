#include "pidwait.h"

int listenfd, connfd;
socklen_t clilen;
pid_t childpid;

void 
str_echo(int sockfd) {
	ssize_t n;
	char buffer[MAXLINE];
		while((n = read(sockfd, buffer, MAXLINE)) > 0){
			s_write(sockfd,buffer,MAXLINE,true);
		}
}


int
main(void)
{
	// Create liste socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	// Fill the struct addr with 0
	memset(&server4_address, 0, sizeof(server4_address)); 
	// Init the struct
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	// Bind the socket 
	Bind(listenfd, (SA* )&server4_address, sizeof(server4_address));
	// Make the sock listen
	Listen(listenfd, LISTENQ);
	// Catch the signal 
	Signal(SIGCHLD, handler_child_wait);

	for( ; ; ) {
		clilen = sizeof(client4_address);
		connfd = Accept(listenfd, (SA *)&client4_address, &clilen);

		if((childpid = Fork())== 0) {
			// child will close the main connection
			// and keed allive just the connection with the client 
			Close(listenfd);
			str_echo(connfd);
			exit(EXIT_SUCCESS);
		}
		// Parrent should close the main connection with the client
		Close(connfd);
	}
}
