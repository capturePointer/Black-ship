#include "lib/net.h"

int main(void)
{
	int ret_bind,ret_listen;
	int main_socket;
	int connection_socket;
	pid_t childpid;
	socklen_t connection_length;
	int n;
	char buf[MAXLINE];

	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);

	ret_bind = bind(main_socket,(SA*)&server4_address,sizeof(server4_address));

	ret_listen = listen(main_socket,LISTENQ);

	while(true)
	{
		connection_length = sizeof(client4_address);
		connection_socket = accept(main_socket,(SA*)&client4_address, &connection_length);
		childpid = fork();
		
		if(childpid == 0)
		{
			close(main_socket);
			while(true)
			{
				n = read(connection_socket,buf,MAXLINE);
				if(n < 0){ printf("Error read"); break;}
				
				int ret_write = write(connection_socket,buf,n);
				if(ret_write < 0){printf("Error write"); break;}
				else break;
			}
			exit(EXIT_FAILURE);
		}
		close(connection_socket);
	}
}
