#include "lib/seilfish.h"

int sockfd;
ssize_t wr_sz;
char buf[]="This is a good day to be alive..";
char *addr = "127.0.0.1";
char buffer[MAXLINE];
int main(void)
{

	sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	initz(&server4_address,0);

	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	Inet_pton(AF_INET, addr, &server4_address.sin_addr);

	Connect(sockfd, (SA*)&server4_address, sizeof(server4_address));
	
	printf("We are now connected to our server..\n");

	wr_sz = write(sockfd, buf,sizeof(buf));

	close(sockfd);
	printf("We are now disconected from the server..\n");

}
