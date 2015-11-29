#include "pidwait.h"

int listenfd, connfd;
socklen_t clilen;
pid_t child;
int
main(void)
{
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&server4_address, 0, sizeof(server4_address));
	
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));
	Signal(SIGCHLD, handler_child_wait);
	Listen(listenfd, LISTENQ);

	for( ; ; ) {
		clilen = sizeof(client4_address);	
		connfd = Accept(listenfd, (SA*)&client4_address, &clilen);


		child = Fork();
		if(child == 0) {
			printf("Childpid\n");
			Close(listenfd);
			str_echo(connfd);
			exit(EXIT_SUCCESS);
		} else if (child >0) {
			printf("Parrent\n");
			Close(connfd);
		}


	}
}
void
str_echo(int sock)
{
	long arg1, arg2;
	ssize_t n;
	char line[MAXLINE];

	for(; ;) {
		if(( n = readline(sock, line, MAXLINE)) == 0)
			return; // conection closed
		if(sscanf(line, "%ld%ld", &arg1, &arg2) == 2) {
			snprintf(line, sizeof(line), "%ld\n", arg1+arg2);
		} else {
			snprintf(line, sizeof(line), "intpu error\n");
		}

		n = (ssize_t)strlen(line);
		s_write(sock, line, (size_t)n, true);
	}
}
