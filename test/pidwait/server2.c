#include "pidwait.h"

int listenfd, connfd;
pid_t childpid;
socklen_t clilen;

int main(void)
{
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&server4_address, 0, sizeof(server4_address));
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;

	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, handler_child_waitpid);

	for (;;) {
		clilen = sizeof(client4_address);

		if((connfd = accept(listenfd, (SA*)&client4_address, &clilen) < 0)) {
			if(errno == EINTR) 
				continue;
			else 
				echo_error("accept error",false,errno);
		}

		fprintf(stdout, "%s", "New client conected\n");

		if( (childpid = fork()) == 0) {
			fprintf(stdout, "%s", "New child\n");
			Close(listenfd);
			fprintf(stdout, "%s","Child Listenfd closed\n");
			str_echo(connfd);
			fprintf(stdout, "%s", "Exit success\n");
			exit(EXIT_SUCCESS);
		}

		Close(connfd);
		fprintf(stdout, "%s", "Parent Connfd closed\n");
	}
}

void str_echo(int sock)
{
	char buffer[MAXLINE];
	ssize_t n;

	while((n = read(sock, buffer, MAXLINE) > 0)){
		printf("\nbuffer = %s \n",buffer);
		s_write(sock, buffer,strlen(buffer), true);
		fprintf(stdout, "%s", "Socket has been written");
	}
}
