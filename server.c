#include "lib/net.h"


void
str_echo(int sockfd)
{
	ssize_t 	n;
	char 		buf[MAXLINE];

	while (true)
	{
		n = s_read(sockfd,buf,MAXLINE,true);
		s_write(sockfd,buf,n,true);

		if(n < 0 && errno == EINTR)
			continue;
		else
			if(n < 0)
				prog_error("read error",true,errno);
		
	}
}
int
main(void)
{
	int 		listenfd,connfd;
	pid_t    	childpid;
	socklen_t 	client_length;


	listenfd = Socket(AF_INET,SOCK_STREAM,0);

	memset(&server4_address,0,sizeof(server4_address));
	server4_address.sin_family		= AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port 		= htons(PORT);
	Bind(listenfd,(SA*) &server4_address ,sizeof(server4_address));
	Listen(listenfd,LISTENQ);
	Signal(SIGCHLD,sig_h_child);
	for(;;)
	{
		client_length = sizeof(client4_address);
		if( (connfd = accept(listenfd,(SA*) &client4_address, &client_length)) < 0 )
		{
			if( errno == EINTR)
				continue;
			else
				prog_error("accept error", true,errno);
		}

		if( (childpid == Fork()) == 0) //child process
		{
			Close(listenfd); // close listen socket
			str_echo(connfd); //process request
			exit(EXIT_SUCCESS);
		}
		Close(connfd); // parrent
	}
}