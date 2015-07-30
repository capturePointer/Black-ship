#include "lib/net.h"

int server_socket,client_socket;
socklen_t client_length;
pid_t pid;




void stream_message(int sockfd);


typedef struct args{
	long arg1;
	long arg2;
}args;

typedef struct result{
	long sum;
}result;



int
main(void)
{
	server_socket = Socket(AF_INET,SOCK_STREAM,0);

	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family 		= AF_INET;
	server4_address.sin_port 		= htons(PORT);
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(server_socket,(SA*)&server4_address ,sizeof(server4_address));

	Listen(server_socket,LISTENQ);

	// just handle the one child

	c_signal(SIGCHLD,handler_child_waitpid);

	while(true)
	{
		client_length = sizeof(client4_address);
		client_socket = Accept(server_socket, (SA*)&client4_address, &client_length);


		if(( pid = Fork()) == 0) // child pid
		{
			//close our main server socket
			printf("Process copil %d\n",getpid());
			Close(server_socket);
			// process the request
			stream_message(client_socket);
			exit(0);
		}
		if(pid > 0) // parrent process
		 {
			printf("Process parinte %d\n",getppid());
			Close(client_socket);
		 }

	}
}

void stream_message(int sockfd)
{
        ssize_t     n;
        args        args;
        result      result;

        for( ; ; ){
            if( (n = read(sockfd, &args, sizeof(args))) == 0 )
                    return; // connection closed by other end
        
            result.sum = args.arg1+args.arg2;
            write(sockfd,&result,sizeof(result));

        }
}
