#include "lib/net.h"
int main_socket;

void stream_message(int sockfd);

int main(int argc, char *argv[])
{
	argc_length(argc,2);

	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET,argv[1],&server4_address.sin_addr);

	Connect(main_socket,(SA *)& server4_address, sizeof(server4_address));

	stream_message(main_socket);
	
	exit(EXIT_SUCCESS);
}

/**
 * 1,4,5  ready for reading
 * 2,7 are ready for writing
 * 1,4 have an exception condition pending
 */
void stream_message(int sockfd)
{
	char sendline[MAXLINE];
	char recvline[MAXLINE];
	

	while(Fgets(sendline,MAXLINE,stdin) != NULL)
	{
		s_write(sockfd,sendline,strlen(sendline),true);
		
		int n = readline(sockfd,recvline,MAXLINE);

		if(n == 0)
			printf("Serverul a terminat prematur");
		
		Fputs(recvline,stdout);
	}
}
