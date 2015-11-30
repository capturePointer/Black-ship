#include "multiplex.h"

int i, maxi, listenfd, connfd, sockfd, nready;
ssize_t n;
char buffer[MAXLINE];
socklen_t clilen;
// We declare OPEN_MAX elements in our array of pollfd structures.
// Determining the maximum number of descriptors that a process can have
// open at ony one time is difficult.
struct pollfd client[OPEN_MAX];



int
main(void) {
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&server4_address, 0, sizeof(server4_address));
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));

	Listen(listenfd, LISTENQ);
	// We use the first entry in the client array for the listening socket
	// and set the descriptor for the remaining entries to -1. We also
	// set the POLLRDNORM event for this descriptor, to be notified by poll
	// when a new connection is ready to be accepted. The variable maxi
	// constains the largest index of the client array currently in use.
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	// Init the client array
	for(i=1; i<OPEN_MAX; i++) {
		client[i].fd = -1;
	}
	// cash the largest index of the array
	maxi = 0;

	for( ; ; ) {
		// We call Poll to wait for either a new connection or data an existing
		// connection. When a new connection is accepted, we find the first
		// avaliable entry in the client array by looking for the first one
		// with a negative descriptor. Notice that we start the search with the
		// index of 1, since client[0] is used for the listening socket.When an
		// available entry is found, we save the descriptor and set the 
		// POLLRDNORM event.
		nready = Poll(client, (unsigned long)maxi+1, INFTIM);

		if(client[0].revents & POLLRDNORM) {
			clilen = sizeof(client4_address);
			connfd = Accept(listenfd, (SA*)&client4_address, &clilen);
			
			for(i=1; i<OPEN_MAX; i++)
				if(client[i].fd < 0) {
					client[i].fd = connfd;
					break;
				}
			if( i == OPEN_MAX)
				prog_error("to many clients, the buffer of clients is full\n",false, errno);
			client[i].events = POLLRDNORM;
			if( i > maxi)
				maxi = i;
			// no more readable fd
			if(--nready <=0)
				continue;
		}
		// The two return events that we check for are POLLRDNORM
		// and POLLERR. The second of thes we did not set in the events
		// member because it is always returned when the condition is true.
		// The reason we check for POLLERR is because some implementations
		// return this event when an RST is received for a connection,
		// while others just return POLLRDNORM. In either case, we call unix
		// read and if an error has occured, it will return an error.
		// When an existing connection is terminated by the client, we just
		// set the fd member to -1
		for(i=1; i<=maxi; i++) {
			if((sockfd = client[i].fd) < 0)
				continue;
			if(client[i].revents & (POLLRDNORM | POLLERR)) {
				if(( n = read(sockfd, buffer, MAXLINE)) < 0) {
					if(errno == ECONNRESET) {
						//close the socket that was cached
						Close(sockfd);
						//free space
						client[i].fd = -1;
					} 
				} else 
					prog_error("read error\n",true,errno);
			} else if(n == 0) {
				Close(sockfd);
				client[i].fd = -1;
			} else 
				s_write(sockfd, buffer,(size_t) n, true);

			if(--nready <=0)
				break;
		}
	}
}
