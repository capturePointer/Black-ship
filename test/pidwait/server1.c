#include "pidwait.h"

int listenfd, connfd;
pid_t childpid;
socklen_t clilen;

void 
str_echo(int sock)
{
	char buffer[MAXLINE];
	ssize_t n;

	while((n = read(sock, buffer, MAXLINE) > 0)){
		s_write(sock, buffer,strlen(buffer), true);
	}
}

int
main(void)
{
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&server4_address, 0, sizeof(server4_address));
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;

	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));

	Listen(listenfd, LISTENQ);
	//Signal handler while(waitpid)
	Signal(SIGCHLD, handler_child_waitpid);

	for (;;) {
		clilen = sizeof(client4_address);
		connfd = accept(listenfd, (SA*)&client4_address, &clilen);

		if(connfd <0) {
			if(errno == EINTR) {
				continue;
			}
			else 
				echo_error("Accept error\n",true,errno);
		}

		if( (childpid = Fork()) == 0) {
			Close(listenfd);
			str_echo(connfd);
			exit(EXIT_SUCCESS);
		}

		Close(connfd);
	}
}


