#include "multiplex.h"
const char *ipaddr = "127.0.0.1";
// TODO: debug
int sockfd;
int
main(void)
{ 
	sockfd = Socket(AF_INET, 0, SOCK_STREAM);
	memset(&server4_address, 0 , sizeof(server4_address));

	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	Inet_pton(AF_INET, ipaddr,&server4_address.sin_addr);

	Connect(sockfd, (SA*)&server4_address, sizeof(server4_address));
	
	str_cli(stdin, sockfd);

	exit(EXIT_SUCCESS);
}

void
str_cli(FILE *stream,int sock)
{
	int		maxfdp1, stdineof;
	fd_set	rset;
	char	buf[MAXLINE];
	int		n;
	// stdineof is a new flag that is initialized to 0
	// As lon as this flag is 0, each time around
	// the main loop, we select on standard input
	// for readability.
	stdineof = 0;
	FD_ZERO(&rset);

	for( ; ; ) {
		if(stdineof == 0)
			FD_SET(fileno(stream), &rset);

		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(stream), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);


		// socket is readable
		// When we read the eof on the socket, if we
		// have already encountered an EOF on stdin,
		// this is normal termination and the function returns.
		// But if we have not yet encountered and EOF on stdin,
		// the server process has prematurely termianted.
		// We now call Read and Write to operate on buffers
		// instead of line and allow select to work
		// for us as expected
		if(FD_ISSET(sockfd, &rset)) {
			if( (n =(int)Read(sock, buf, MAXLINE)) == 0) {
				if(stdineof == 1) 
					return;
				else 
					prog_error("str_cli error", true, errno);
				Write(fileno(stdout), buf, (size_t)n);
			}
		}


		// input is readable
		// When we encounter the EOF on stdin, our new flag stdineof is
		// set and we call shutdown with a second arg of SHUT_WR to send 
		// the FIN. Here also we've changed to operating on
		// buffers instead of lines using Read and s_write
		if(FD_ISSET(fileno(stream), &rset)) {
			if((n = (int)Read(fileno(stream), buf, MAXLINE)) == 0) {
				stdineof = 1;
				Shutdown(sock, SHUT_WR); 
				FD_CLR(fileno(stream), &rset);
				continue;
			}
			s_write(sock, buf,(size_t) n,true);
		}
	}
}
