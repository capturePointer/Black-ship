#include "lib/sailfish.h"

int listenfd, n, connfd;
socklen_t len;
char buff[MAXLINE];
time_t ticks;
struct sockaddr_storage client_address;

int main(int argc, char **argv)
{
	if( argc != 2){
		printf("Usage , ./server <service or port>\n");
		exit(EXIT_FAILURE);
	}
	
	listenfd = host_tcp(NULL, argv[1], NULL);

	for( ;; ) {
		len = sizeof(client_address);
		connfd = Accept(listenfd, (SA *)&client_address, &len);

		printf("connection from %s\n", proto_ntop( connfd, (SA*)&client_address, len) );

		ticks = time (NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));
		
		Close(connfd);
	}
}
