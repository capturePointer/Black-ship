#include "multiplex.h"
//
// Unfortunately, there is a problem with the server that we just showed.
// Consider what happens if a malicious client connects to the server, sends
// one byte of data(other than a newline), and then goes to sleep. The server
// will call read, which will read the single byte of data from the client
// and then block in the next call to read, waiting for more data from this
// client.The server is then blocked/hung by this one client and will not
// service any other clients(either new client connections or existing client
// data) until the malicious client either sends a newline or terminates.
//
// The basic concept here is that when a server is handling multiple clients
// the server can never block in a function call related to a single client.
// Doing so can hang the server and deny sevice to all other clients.
//
// Possible solutions are to
//				- use noblocking I/O
//				- have each client serviced by a separate thread of control
//				- place a timeout on the I/O operations

int			sockfd, listenfd, connfd, nready, 
			client[FD_SETSIZE],i ,maxi, maxfd;
socklen_t	clilen;
ssize_t		n;
fd_set		rset, allset;
char		buf[MAXLINE];

int 
main(void)
{
	//create listen socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	// fill addr with 0
	memset(&server4_address, 0, sizeof(server4_address));
	// init addr
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind listenfd
	Bind(listenfd, (SA*)&server4_address, sizeof(server4_address));
	//listen mode on listenfd
	Listen(listenfd, LISTENQ);
	// init	
	maxfd = listenfd;
	// index into client[] array
	maxi = -1; 
	// Fill all all the clients of lenght of FD_SETSIZE
	for(i = 0; i< FD_SETSIZE; i++) 
		client[i] = -1;

	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	// Select waits for something to happen: either the establishment
	// a new client connection of the arrival of data, a FIN, or RST 
	// on an existing connection
	for(;;) {
		rset = allset;
		// We interested just in  our readfds
		nready = Select(maxfd + 1, &rset, NULL,NULL,NULL);
		// If the listening socket is readable, a new connection
		// has been established.We call accept and update our data
		// structures accordingly.We use the first unused entry in the
		// client array to record the connected socket.
		// The number of ready  descriptors is decremented, and if it
		// is 0, we can avoid the next for loop. This lets us use the return
		// value from select to avoid checking descriptors that are not ready.
		if(FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(client4_address);
			connfd = Accept(listenfd, (SA*)&client4_address, &clilen);

			for(i = 0; i<FD_SETSIZE; i++){
				if(client[i] < 0) {
					client[i] = connfd; // save descriptor
					break;
				}
			}
			// If we already fill up the limit of clients
			// Throw error
			if( i == FD_SETSIZE)
				prog_error("too many clients",true,errno);
			
			// ELSE if we not reach the limit
			//
			// add a new descriptor
			FD_SET(connfd, &allset);

			if(connfd > maxfd)
				maxfd = connfd;

			if(i >maxi)
				maxi = i;
			if(--nready <=0)
				continue;
		}
		//A test is made for each existing client connection 
		//as to whether or not its descriptor is in the descripor
		// set returned by select.If so, a line is read from the client
		// and echoed back to the client.If the client does the connection, read returns 0
		// and we update our data structures accordingly.
		// We never decrement the value of maxi, but we could check for the 
		// possibility each time a client closes it's connection.
		// This server avoids all the overhead of creating a new process for each client and it is a nice
		// example of select.
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
