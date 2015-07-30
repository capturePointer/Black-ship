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
    char        sendline[MAXLINE];
    args        args;
    result      result;


    while(Fgets(sendline,MAXLINE,stream) != NULL)
    {
    
        if(sscanf(sendline,"%ld%ld",&args.arg1,&args.arg2) != 2){
            printf("invalid input: %s", sendline);
            continue;
        }
        write(sockfd,&args,sizeof(args));
        if(readline(sockfd,&result,sizeof(result)) == 0){
            prog_error("str_cli: server termianted prematurely",true,errno);
        }

        printf("%ld\n",result.sum);
    }

}
