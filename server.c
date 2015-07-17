#include <stdio.h>
/*sockaddr_in*/
#include <netinet/in.h>
#include <time.h>
#include <string.h>
/*sock types*/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
/*close*/
#include <unistd.h>
#define MAXLINE 255
#define PORT 13
int client_socket,server_socket;
socklen_t client_len_struct;
struct sockaddr_in server,client;
char buffer[MAXLINE];
time_t ticks;

int 
main()
{
	server_socket = socket(AF_INET,SOCK_STREAM,0);

	if(server_socket < 0){printf("Nu am putut creasocketul");}

	memset(&server,0,sizeof(server));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	int ret_bind = bind(server_socket,(struct sockaddr*)&server,sizeof(server));
	if(ret_bind < 0){printf("Nu am putut face bindu-ul corect");}

	int ret_listen = listen(server_socket,5);
	if(ret_listen <0){printf("Nu putem face socketul sa asculte");}
	
	for(;;){
	
		client_len_struct = sizeof(client);
		
	    client_socket = accept(server_socket,(struct sockaddr*)&client,&client_len_struct);
		
		printf("Connection from %s port %d\n",
				inet_ntop(AF_INET,&client.sin_addr,buffer,sizeof(buffer)),
				ntohs(client.sin_port));

		ticks = time(NULL);
		snprintf(buffer,sizeof(buffer),"%.24s\r\n",ctime(&ticks));
		int ret_write = write(client_socket,buffer,strlen(buffer));
		if(ret_write < 0){printf("Am belito");}
		close(client_socket);

	
	}

}
