#include "lib/net.h"
void
str_cli(FILE *stream,int sockfd)
{
	char *rbyte;
	char sendline[MAXLINE],recvline[MAXLINE];
	while( (rbyte = Fgets(sendline,MAXLINE,stream)) != NULL)
	{
		s_write(sockfd,sendline,strlen(sendline),true);
		if(readline(sockfd,recvline,MAXLINE) == 0)
			prog_error("str_cli : server terminated prematurely",true,errno);
		Fputs(recvline,stdout);
	}	
}
int
main(int argc, char *argv[])
{
	int 			sockfd;
	argc_length(argc,2);
	sockfd = Socket(AF_INET,SOCK_STREAM,0);
	memset(&server4_address,0,sizeof(server4_address));

    server4_address.sin_family 		= AF_INET;
	server4_address.sin_port 		= htons(PORT);
	
    Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);
	
    Connect(sockfd, (SA *) &server4_address ,sizeof(server4_address));
	
    str_cli(stdin,sockfd);
	exit(0);

}
