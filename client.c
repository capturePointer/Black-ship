#include "lib/net.h"



void
str_cli(FILE *stream,int sockfd)
{
	char *rbyte;
	char sendline[MAXLINE],recvline[MAXLINE];

	while(true)
	{
		rbyte = Fgets(sendline,MAXLINE,stream);
		if(rbyte == NULL) prog_error("fgsets error",true,errno);

		s_write(sockfd,sendline,strlen(sendline),true);
		s_read(sockfd,recvline,MAXLINE,true);
		
		

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
	connect(sockfd, (SA *) &server4_address ,sizeof(server4_address));
	str_cli(stdin,sockfd);
	exit(0);

}