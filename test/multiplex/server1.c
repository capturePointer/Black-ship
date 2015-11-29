#include "multiplex.h"

int listenfd, connfd;
socklen_t clilen;
int main(void) {
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&server4_address, 0, sizeof(server4_address));
	
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));

	Listen(listenfd, LISTENQ);
	
	Signal(SIGCHLD, handler_child_wait);

	for(;;) {
		clilen = sizeof(client4_address);
		
		connfd = Accept(listenfd, (SA*)&client4_address, &clilen);

		printf("We have a new client\n");

		pid_t childpid = Fork();

		if (childpid == 0) {
			Close(listenfd);
			str_echo(connfd);
			exit(EXIT_SUCCESS);
		}
		Close(connfd);
	}
}

void
str_echo(int sockfd)
{
	char buffer[MAXLINE];
	ssize_t n;

	while((n=read(sockfd, buffer, MAXLINE)) > 0) {
		s_write(sockfd, buffer, (size_t)n, true);
	}
}

