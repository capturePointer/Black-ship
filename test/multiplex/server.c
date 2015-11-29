#include "multiplex.h"
// TODO: DEBUG
int connfd, listenfd;
int nready, client[FD_SETSIZE];
int sockfd;
socklen_t clilen;
int i, maxi, maxfd;
ssize_t n;
fd_set rset, allset;
char buf[MAXLINE];

int 
main(void)
{
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&server4_address, 0, sizeof(server4_address));

	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));
	
	Listen(listenfd, LISTENQ);
	
	maxfd = listenfd;
	maxi = -1; 
	for(i = 0; i< FD_SETSIZE; i++) 
		client[i] = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for(;;) {
		rset = allset;
		nready = Select(maxfd + 1, &rset, NULL,NULL,NULL);
		// new client conn
		if(FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(client4_address);
			connfd = Accept(listenfd, (SA*)&client4_address, &clilen);

			for(i = 0; i<FD_SETSIZE; i++){
				if(client[i] < 0) {
					client[i] = connfd;
					break;
				}
			}

			if( i == FD_SETSIZE)
				prog_error("too many clients",true,errno);

			// add a new descriptor
			FD_SET(connfd, &allset);

			if(connfd > maxfd)
				maxfd = connfd;

			if(i >maxi)
				maxi = i;

			if(--nready <=0)
				continue;
		}

		for(i=0;i<=maxi; i++) {
			if((sockfd = client[i])<0)
				continue;
			if(FD_ISSET(sockfd, &rset)) {
				if((n = Read(sockfd, buf, MAXLINE)) == 0) {
					//conn closed/
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else {
					s_write(sockfd, buf, (size_t)n, true);
				}
				if( --nready <=0)
					break;
			}
		}

	}
}
