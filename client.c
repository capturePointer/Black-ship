#include "lib/net.h"

int main_socket;
int connection_socket;
char *byte_size;

int
main(int argc, char *argv[])
{
	argc_length(argc,2);
	
	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	
	memset(&server4_address,0,sizeof(server4_address));
	
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = PORT;

	Inet_pton(AF_INET,argv[1],&server4_address.sin_addr);

	connection_socket = Connect(main_socket,(SA *)&server4_address, sizeof(server4_address));
	
	while(true)
	{
		char send_line[MAXLINE],recive_line[MAXLINE];		
		Fgets(send_line,MAXLINE,stdin);
				
	}

}
