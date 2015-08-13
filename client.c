#include "lib/net.h"
int main_socket;

void stream_message(FILE *stream, int sockfd);

int main(int argc, char *argv[])
{
	argc_length(argc,2);

	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET,argv[1],&server4_address.sin_addr);

	Connect(main_socket,(SA *)& server4_address, sizeof(server4_address));

	stream_message(stdin,main_socket);
	
	exit(EXIT_SUCCESS);
}

/**
 * 1,4,5  ready for reading
 * 2,7 are ready for writing
 * 1,4 have an exception condition pending
 */
void stream_message(FILE *stream, int sockfd)
{
	int		maxfdp1, stdineof;
	fd_set	rset;
	char	buf[MAXLINE];
	int		n;
	//we note that end of file is 0
	stdineof = 0;
	// we set all bytes in rset to be 0
	FD_ZERO(&rset);

	for( ; ; )
	{
		if(stdineof == 0)
			FD_SET(fileno(stream), &rset);
		FD_SET(sockfd,&rset);
		maxfdp1 = max(fileno(stream), sockfd) + 1;
		//readfds,writefd,timeval to null
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if(FD_ISSET(sockfd, &rset))
		{
			if((n = Read(sockfd, buf, MAXLINE)) ==0 )
			{	if(stdineof == 1)
					return; /*normal termination*/
				else
					prog_error("stream_message: server terminated prematurely",true,errno);
			}
			//write to the standard output like Fputs(buf,stdout);
			Write(fileno(stdout), buf, n);
		}	 
		
		if(FD_ISSET(fileno(stream), &rset))
		{
			if((n = Read(fileno(stream),buf,MAXLINE)) == 0)/*input is readble*/
			{
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR); /*send fin */
				FD_CLR(fileno(stream), &rset);
				continue;
			}
			s_write(fileno(stream),buf,n,true);
		}
	}
}
