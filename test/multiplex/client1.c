#include "multiplex.h"

int			sock;
const char *ip = "127.0.0.1";

int
main(void)
{
	sock = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&server4_address, 0 , sizeof(server4_address));
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET, ip, &server4_address.sin_addr);
	Connect(sock, (SA*)&server4_address, sizeof(server4_address));
	str_cli(stdin, sock);
}
// If the peer TCP sends data, the socket becomes readable and read returns
// greater than 0(number of bytes of data).
//
// If the peer TCP send FIN(the peer process termiantes),the socket
// becomes readable and read returns 0(EOF).
//
// If the peer TCP sends RSTthe peer host has crashed and rebooted), the
// socket becomes readable, read returns -1, and errno contains the specific
// error code.
void
str_cli(FILE *stream, int sockfd)
{
	// stdineof is a new flag that initialized to 0.
	// as long as this flag is 0 each time around
	// the main loop, we select input for readability
	int		maxfdp1, stdineof, n;
	fd_set	rset;
	char	buffer[MAXLINE];

	FD_ZERO(&rset);
	stdineof = 0;
	for(; ;) {
		if(stdineof == 0)
			FD_SET(fileno(stream), &rset);
		FD_SET(sockfd, &rset);

		maxfdp1= max(fileno(stream),sockfd) +1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);
		// When we read EOF in the socket, if we have already encountered and
		// EOF on standard input this is normal termination and the functin
		// return. But if we have not yes encountered and EOF on standard input
		// the server process as prematurely terminated.We now call
		// read andn write to operate on buffers instead of lines and allow
		// select to work for as as expected.
		if( FD_ISSET(sockfd, &rset)) {
			if((n = (int)Read(sockfd, buffer, MAXLINE)) == 0) {
				if(stdineof == 1) 
					return;
				else
					printf("server termainted prematurely\n");
			}
			Fputs(buffer, stdout);
		}
		// When we encounter the EOF on the stdin, our new flag,
		// stdineof is set and we call shutdown with a second argument of
		// SHUT_WR to send the fin.Here also,
		// we've changed to operating on buffers instead of line
		// using read and safe_write(s_write)
		if(FD_ISSET(fileno(stream), &rset)) {
			if( (n = (int)Read(fileno(stream), buffer, MAXLINE)) == 0) {
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR); 
				FD_CLR(fileno(stream), &rset);
				continue;
			}
			s_write(sockfd, buffer, (size_t)n, true);
		}
	}

}
