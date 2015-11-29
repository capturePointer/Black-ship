#include "pidwait.h"

int sockfd;

int
main(int argc, char *argv[])
{
	argc_length(argc, 2);

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&server4_address, 0, sizeof(server4_address));
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET,argv[1], &server4_address.sin_addr);

	Connect(sockfd, (SA*)&server4_address, sizeof(server4_address));
	str_cli(stdin, sockfd);
	exit(EXIT_SUCCESS);
}

void
str_cli(FILE *stream, int sock)
{
	char sendline[MAXLINE];
	struct args args;
	struct result result;
	
	while(Fgets(sendline,MAXLINE, stream) != NULL) {
		//fiil the buffers args.arg1, args.arg2
		if(sscanf(sendline,"%ld%ld", &args.arg1, &args.arg2) != 2) {
			printf("invalid input: %s", sendline);
			continue;
		}
		// send the struct
		s_write(sock, &args, sizeof(args),true);
		
		if(s_read(sock, &result, sizeof(result),true)== 0)
			prog_error("str_cli: server terminated prematurely",true,errno);

		printf("%ld\n", result.sum);
	}
}
