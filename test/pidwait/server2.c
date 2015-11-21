#include "pidwait.h"

int listenfd;
int connfd;
socklen_t clilen;
time_t ticks;
char buff[MAXLINE];
int main(void)
{
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&server4_address, 0, sizeof(server4_address));

	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(13);
	server4_address.sin_family = AF_INET;

	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, handler_child_waitpid);
	
	for (;;) {
		clilen = sizeof(client4_address);

		connfd = Accept(listenfd, (SA*)&client4_address, &clilen);
		printf("Connection form %s port %d\n", 
				Inet_ntop(AF_INET, &client4_address.sin_addr, buff, MAXLINE),
			   	ntohs(client4_address.sin_port));
		ticks = time(NULL);
		snprintf(buff, sizeof(buff),"%.24s\r\n", ctime(&ticks));
		s_write(connfd, buff, strlen(buff), true);

		Close(connfd);
	}	
}
