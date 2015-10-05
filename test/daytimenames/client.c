#include "lib/seilfish.h"

int 				sockfd,n;
char 				recvline[MAXLINE +1];
struct in_addr 		**pptr;
struct in_addr 		*inetaddrp[2];
struct in_addr 		inetaddr;
struct hostent 		*hp;
struct servent 		*sp;

int main(int argc, char **argv)
{
	if(argc != 3){
		echo_error("usage ./clinet <hostname> <service>",false,0);
		exit(EXIT_FAILURE);
	}
	if( (hp = gethostbyname(argv[1])) == NULL){
		if(inet_aton(argv[1], &inetaddr) == 0){
			printf("hostname error for %s : %s",argv[1], hstrerror(h_errno));
			exit(EXIT_FAILURE);
		} else {
			inetaddrp[0] = &inetaddr;
			inetaddrp[1] = NULL;
			pptr = inetaddrp;
		}
	} else {
		pptr = (struct in_addr **) hp->h_addr_list;
	}

	if( (sp = getservbyname	(argv[2],"tcp")) == NULL)
		printf("getservbyname error for %s", argv[2]);

	for( ; *pptr != NULL; pptr++ ){
		sockfd = Socket(AF_INET,SOCK_STREAM, 0);

		initz(&server4_address,0);
		server4_address.sin_family = AF_INET;
		server4_address.sin_port = sp->s_port;
		memcpy(&server4_address.sin_addr, *pptr, sizeof(struct in_addr));
		
		printf("trying %s\n",
			proto_ntop(sockfd, (SA*)&server4_address,sizeof(server4_address)));

		
		if(connect(sockfd, (SA*)&server4_address, sizeof(server4_address)) == 0){
			break;
		}
		
		printf("connection error");
		Close(sockfd);
	}

	if(*pptr == NULL) {
		printf("Unable to connect\n");
		exit(EXIT_FAILURE);
	}

	while( (n = Read(sockfd, recvline, MAXLINE)) > 0){
		recvline[n] = 0;
		Fputs(recvline,stdout);
	}
	exit(EXIT_SUCCESS);
}
