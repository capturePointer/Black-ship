#include "lib/seilfish.h"
#include <time.h>

/**
 * Our data types
 *
 */
int						listensockfd, connsockfd;
socklen_t				len;
char					buffer[MAXLINE];
time_t					ticks;
struct sockaddr_storage cliaddr;

int
main(void) {
	listensockfd = Socket(AF_INET, SOCK_STREAM, 0);
	initz(&server4_address, 0);
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(listensockfd, (SA*)&server4_address, sizeof(server4_address));
	Listen(listensockfd, LISTENQ);

	for(;;){
		len = sizeof(client4_address);
		connsockfd = Accept(listensockfd, (SA*)&client4_address, &len);
        printf("Connection from %s\n", proto_ntop(connsockfd, (SA*)&
			 client4_address, len));
		ticks = time(NULL);
		snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
		Write(connsockfd, buffer, strlen(buffer));
        Close(connsockfd);
	}
}
