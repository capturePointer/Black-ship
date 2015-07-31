#include "lib/net.h"

int client_socket;

void stream_message(FILE *stream,int sockfd);


typedef struct args{
	long arg1;
	long arg2;
}args;

typedef struct result{
	long sum;
}result;

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
    int     maxfdp1,stdineof;
    fd_set  rset;
    char    buf[MAXLINE];
    int     n;

    stdineof = 0;
    FD_ZERO(&rset);

    for( ; ; )
    {
        if(stdineof == 0)      
            FD_SET(fileno(stream), &rset);
        
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(stream), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        
        if(FD_ISSET(sockfd, &rset)) /*socket is readble*/ 
        { 
            if( (n=readline(sockfd,buf,MAXLINE)) == 0)
            {
               if(stdineof == 1)
                   return;
                else
                    prog_error("Readline error",true,errno);
             s_write(fileno(stdout),buf,n,true);
            }         
        }
       
        if(FD_ISSET(fileno(stream),&rset)) /*input is readdble*/
            if( (n = readline(fileno(stream),buf,MAXLINE)) == 0)
            {
                stdineof = 1;
                Shutdown(sockfd,SHUT_WR); /*sends a fin*/
                FD_CLR(fileno(stream),&rset);
                continue;
            }
        s_write(sockfd,buf,n,true);
    }
}
