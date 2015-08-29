#include "lib/net.h"
int			listenfd, connfd, udpfd, nready, maxfdp1;
char		mesg[MAXLINE];
pid_t		childpid;
fd_set		rset;
ssize_t		n;
socklen_t   len;
const int on = 1;
void sig_chld(int);
int
main(void)
{
	// create listening socket TCP
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	//bind the struct sockaddr with 0
	memset(&server4_address,0,sizeof(server4_address));
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	Bind(listenfd, (SA*)&server4_address,sizeof(server4_address));
	Listen(listenfd,LISTENQ);

	/*create udp socket*/
	udpfd = Socket(AF_INET,SOCK_DGRAM,0);
	memset(&server4_address,0,sizeof(server4_address));
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(udpfd,(SA*)&server4_address,sizeof(server4_address));

	Signal(SIGCHLD,handler_child_wait);
	FD_ZERO(&rset);
	maxfdp1 = max(listenfd,udpfd) + 1;
	for(; ;)
	{
		FD_SET(listenfd,&rset);
		FD_SET(udpfd, &rset);

		if( (nready = select(maxfdp1, &rset, NULL,NULL, NULL)) <0 )
			if(errno == EINTR)
				continue;
			else
				prog_error("select error",true,errno);
		
		
		if(FD_ISSET(listenfd, &rset))
		{
			len = sizeof(client4_address);
			connfd = Accept(listenfd, (SA*)&client4_address, &len);

			if(( childpid = Fork() == 0) // child process
			{
				Close(listenfd);
				str_echo(connfd);
				exit(0);
			}
			else
				Close(connfd);
		}
		if(FD_ISSET(udpfd, &rset))
		{
			len = sizeof(client4_address);
			n = Recvfrom(udpfd, mesg, MAXLINE, 0, (SA *)& client4_address, &len);

			Sendto(udpfd,mesg, n, 0, (SA*)&client4_address, len);
		}
	}
}
