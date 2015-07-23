#include "lib/net.h"

void str_echo(int connection_socket);

int main_socket;
int connection_socket;
socklen_t connection_length;
char buf[MAXLINE];
pid_t childpid;



int
main(void)
{
	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);
	
	Bind(main_socket,(SA*)&server4_address,sizeof(server4_address));
	Listen(main_socket,LISTENQ);
	
	connection_length = sizeof(client4_address);

	while(true)
	{
		connection_socket = Accept(main_socket,(SA*)&client4_address, &connection_length);
		
		childpid = Fork();

		if(childpid == 0)
		{
			Close(main_socket);
			str_echo(connection_socket);
			exit(0);
		}
		Close(connection_socket);
	}
}
void str_echo(int connection_socket)
{
	while(true)
	{
		s_read(connection_socket,buf,MAXLINE,true);	
		s_write(connection_socket,buf,MAXLINE,true);				
		
		if(strncmp(buf,"exit\n\0",strlen(buf)) == 0) break;
	}
}