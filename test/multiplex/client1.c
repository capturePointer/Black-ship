#include "multiplex.h"

const char *ip = "127.0.0.1";
int sockfd;


int main(void) {

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&server4_address, 0 , sizeof(server4_address));

	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;

	Inet_pton(AF_INET, ip, &server4_address.sin_addr);

	Connect(sockfd, (SA*)&server4_address, sizeof(server4_address));
	str_cli(stdin, sockfd);
	exit(EXIT_SUCCESS);
}

void str_cli(FILE *fp, int sock) {
	int maxfdp1;
	char sendline[MAXLINE], recvline[MAXLINE];
	fd_set rset;

	FD_ZERO(&rset);
	for ( ; ; ) {

		FD_SET(fileno(fp), &rset);
		FD_SET(sock, &rset);
		maxfdp1 = max(fileno(fp), sock) + 1;

		Select(maxfdp1, &rset, NULL, NULL, NULL);
		
		if(FD_ISSET(sockfd, &rset)) {
			if(readline(sock, recvline, MAXLINE) == 0) 
				prog_error("str_cli: server termianted prematurely",true, errno);
			Fputs(recvline, stdout);
		}

		if(FD_ISSET(fileno(fp), &rset)) {
			if(Fgets(sendline, MAXLINE, fp) == NULL){
				return;
			}
			s_write(sock, sendline, strlen(sendline),true);
		}
	}
}
