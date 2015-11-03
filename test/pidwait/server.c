#include "lib/sailfish.h"

int sockfd, clientfd;
pid_t childpid;
socklen_t client_len;

void
str_echo(int sockfd)
{
	long 			arg1, arg2;
	ssize_t 		n;
	char			line[MAXLINE];

	for( ; ; ) {
		if(( n = readline(sockfd, line, MAXLINE)) == 0) {
			return ; // connection close by other end
		}
		if(sscanf(line, "%ld%ld", &arg1, &arg2) == 2) {
			snprintf(line, sizeof(line), "%ld\n", arg1+arg2);
		} else {
			snprintf(line, sizeof(line), "input error\n");
		}
		n = strlen(line);
		s_write(sockfd, line, n,true);
	}
}
int main(void) {

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&server4_address, 0 , sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);

	Bind(sockfd, (SA*)&server4_address, sizeof(server4_address));

	Listen(sockfd, LISTENQ);

	Signal(SIGCHLD, handler_child_wait);

	for( ; ; ) {
		client_len = (socklen_t)sizeof(client4_address);
		if((clientfd = accept(sockfd, (SA*)&client4_address, &client_len) < 0)){
			if(errno == EINTR) {
				continue;
			}
			else
				prog_error("Error accept",true,errno);
		}
		//child pid
		if ((childpid = Fork()) == 0) {
			Close(sockfd);
			str_echo(clientfd);
			exit(0);
		}
		Close(clientfd);

	}
}