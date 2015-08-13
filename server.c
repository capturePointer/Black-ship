#include "lib/net.h"
int main_socket, client_socket,maxfd,maxi;
int nready,client[FD_SETSIZE];
ssize_t n;
fd_set rset, allset;
socklen_t client_len;
int i;
int sockfd;
char buf[MAXLINE];
int main()
{

	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	
	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);

	Bind(main_socket, (SA *)&server4_address,sizeof(server4_address));
	
	Listen(main_socket,LISTENQ);

	//*F_SET
	maxfd = main_socket;
	maxi = -1;
	//we will make all 256 elements to -1;
	//-1 means that we don't have any client connected yet
	for(i=0;i<FD_SETSIZE;i++)
		client[i] = -1;

	FD_ZERO(&allset);
	FD_SET(main_socket, &allset);

	for(;;)
	{
		rset = allset;
		nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);

		// we have a new client connection
		if(FD_ISSET(main_socket, &rset))
		{
			client_len = sizeof(client4_address);
			client_socket = Accept(main_socket, (SA *)&client4_address,&client_len);
			//mark our new conneciton
			for(i = 0; i< FD_SETSIZE; i++)
			{
				if(client[i] < 0)
				{
					client[i] = 0;
					break;
				}
			}
			if(i == FD_SETSIZE)
				prog_error("to many clients",true,errno);

			//*after we return our new connection (client_connection) throwgh accept
			//we must mark in our fd_set;
			FD_SET(client_socket, &allset);
			
			if(main_socket > maxfd)
				maxfd = main_socket; /*for select */
			if(i > maxi)
				maxi = i; // max index in cliednt[] array
			if(--nready <= 0)
				continue; /*no more readble descriptors */
		}
		for(i = 0; i<maxi; i++) // check all clients for data
		{
			if( (sockfd= client[i]) <0 )
				continue;
			if( FD_ISSET(sockfd, &rset))
			{
				if( (n = Read(sockfd, buf, MAXLINE)) ==0 )
				{
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				else
					s_write(sockfd, buf, n,true);

				if( --nready <=0 )
					break; // no more readble descriptors
			}
		}
	}
}
