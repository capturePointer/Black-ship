#include "lib/net.h"
int sockfd;
socklen_t len;
int main(int argc, char *argv[])
{

	argc_length(argc,2);

	sockfd = Socket(AF_INET,SOCK_DGRAM,0);
	
	memset(&server4_address,0,sizeof(server4_address));

	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET,argv[1],&server4_address.sin_addr);

	Connect(sockfd,(SA*)&server4_address,sizeof(server4_address));

	len = sizeof(client4_address);
	Getsockname(sockfd,(SA*) &client4_address, &len);
	
	printf("local addres %s\n", proto_ntop(sockfd,(SA*)&client4_address,len));

	
	exit(EXIT_SUCCESS);

}
