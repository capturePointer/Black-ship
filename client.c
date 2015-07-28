#include "lib/net.h"

int client_socket;

void stream_message(FILE *stream,int sockfd);

int
main(int argc, char *argv[])
{

	argc_length(argc,2);

	client_socket = Socket(AF_INET,SOCK_STREAM,0);

	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family		= AF_INET;
	server4_address.sin_port 		= htons(PORT);
	Inet_pton(AF_INET,argv[1],&server4_address.sin_addr);

	Connect(client_socket,(SA*)&server4_address,sizeof(server4_address));

	while(true)
	{
		stream_message(stdin,client_socket);
		exit(0);
	}
}
void stream_message(FILE *stream,int sockfd)
{
	char *n;
	char sendline[MAXLINE];
	char recvline[MAXLINE];

	while( (n = Fgets(sendline,MAXLINE,stream)) != NULL)
	{
		s_write(sockfd,sendline,MAXLINE,true);
		s_read(sockfd,recvline,MAXLINE,true);
		Fputs(recvline,stdout);
	}
}