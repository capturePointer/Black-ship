#include "lib/net.h"

int			main_socket, client_socket,maxfd,maxi,sockfd;
int			nready;
ssize_t		n;
char		buf[MAXLINE];
socklen_t	client_length;
int			i;


/**for poll
 *
 * @struc
 */

struct pollfd client[OPEN_MAX];
int main()
{

	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	
	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);

	Bind(main_socket, (SA *)&server4_address,sizeof(server4_address));
	
	Listen(main_socket,LISTENQ);
	
	client[0].fd = main_socket;
	client[0].events = POLLRDNORM;// 64 1000000
	for(i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1;
	maxi = 0;

	for(;;)
	{
		nready = Poll(client, maxi+1, INFTIM);

		if(client[0].revents & POLLRDNORM) //and byte
		{
			client_length = sizeof(client4_address);
			client_socket = Accept(main_socket, (SA *)&client4_address, &client_length);
			
			for(i = 1; i<OPEN_MAX; i++)
					if(client[i].fd < 0)
					{
						client[i].fd = client_socket;
						break;
					}

				if( i == OPEN_MAX )
						prog_error("to many clients",false,errno);

				client[i].events = POLLRDNORM;
				if( i > maxi)
					maxi = i;

				if(--nready <= 0)
					continue;
		}
		for(i = 1; i<= maxi; i++)
		{
			if( (sockfd = client[i].fd) <0 )
				continue;
			if(client[i].revents & (POLLRDNORM | POLLERR))
			{
				if( (n = read(sockfd,buf,MAXLINE)) < 0 )
				{
					if(errno == ECONNRESET)
					{
						printf("Client aborded connection\n");
						Close(sockfd);
						client[i].fd = 1;
					}
					else
						printf("read error");
				}
				else
					if (n == 0) // connection close by client
					{
						Close(sockfd);
						client[i].fd = -1;
					}
					else
						s_write(sockfd,buf,n,false);
					
					if( --nready <=0 )
						break;
			}
		}
	}
}
