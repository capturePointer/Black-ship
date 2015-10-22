#include "lib/sailfish.h"



int sockfd,clisockfd;
socklen_t len;
ssize_t rd_sz;
int flags;
struct msghdr message;
struct iovec iov[1];
char buf[MAXLINE];

int main(void)
{
	sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	initz(&server4_address, 0);
	
	server4_address.sin_port = htons(PORT);
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(sockfd, (SA *)&server4_address, sizeof(server4_address));

	Listen(sockfd, LISTENQ);

	for(;;){
	  len = (socklen_t) sizeof(client4_address);
      clisockfd = Accept(sockfd, (SA*)&client4_address, &len);
	  printf("We have a new client..\n");

	  rd_sz = read(clisockfd, buf,sizeof(buf));
	  
	  
	  printf("We have a message\n");
	  printf("The message is:\n");
	  printf("%s\n",buf);

	  if(read(clisockfd, buf, sizeof(buf) <0))
			close(clisockfd);
	  printf("Client exits..\n");
	}



}
