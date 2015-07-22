#include "lib/net.h"

int main_socket;
char *byte_size;

int
main(int argc, char *argv[])
{
	argc_length(argc,2);
	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	memset(&server4_address,0,sizeof(server4_address));
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET,argv[1],&server4_address.sin_addr);

	Connect(main_socket, (SA *)&server4_address, sizeof(server4_address));
	char sendline[MAXLINE], recvline[MAXLINE];

	while(true)
	{
		// scanf("%s",sendline);
		byte_size = Fgets(sendline,MAXLINE,stdin);
		if(byte_size == NULL) break;

		s_write(main_socket,sendline,MAXLINE,true);
		s_read(main_socket,recvline,MAXLINE,true);
		Fputs(recvline,stdout);		
		sendline[strlen(sendline)] = '\0';
		if(strncmp(recvline,"exit\0",sizeof(recvline)) == 0)
		 	break;
	}
	exit(EXIT_SUCCESS);
}
