#include "lib/sailfish.h"

int listenfd, n, connfd;
socklen_t len;
char buff[MAXLINE];
time_t ticks;
struct sockaddr_storage client_address;
socklen_t addrlen;

int 
main(int argc, char **argv)
{
	if(argc == 2) 
		listenfd = host_tcp(NULL, argv[1], &addrlen);
	else 
		if (argc == 3)
			listenfd = host_tcp(argv[1],argv[2], &addrlen); // addrlen will 
												//return the length of the 
												//struct addr conn
		else {
			printf("Usage , ./server <service or port>\n");
			exit(EXIT_FAILURE);
		}

	for( ;; ) {
		len = sizeof(client_address);
		connfd = accept(listenfd, (SA *)&client_address, &len);
        if(errno == ENOTSOCK) {
			printf("Here it brokes\n");
			exit(EXIT_FAILURE);
        }
		printf("connection from %s\n", proto_ntop( connfd, (SA*)&client_address, len));

		ticks = time (NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));
		
		Close(connfd);
	}
}
