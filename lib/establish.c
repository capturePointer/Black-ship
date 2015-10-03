#include "seilfish.h"
/**
 * The function(and our other functions that prove a simple interface
 * to getaddrinfo in the following sections), terminates if either
 * gataddrinfo fails or no call to connect succeeds.The only
 * return is upon success.It would be ahrd to return an error code
 * (one of the EAI_XXX constants) without adding another argument.
 */

int 
establish_tcp(const char *host, const char *server)
{
	int sockfd;
	addrinfo hints, *res, *init;
	initz(&hints, 0);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype= SOCK_STREAM;
	/**
	 * Getaddrinfo is called one and we specify the addr
	 * family as AF_UNSPEC and the socket type as SOCK_STREAM
	 */
	int n = getaddrinfo(host, server, &hints, &res);
    if(n != 0){
		printf("Nasol");
		exit(EXIT_FAILURE);
    }
	//shallow copy
	init = res;

    /**
     * Each returned IP address is then tried.
     * socket() and connect() are called.It is noa a fatal error
     * for socket to fail, as this could happen if an IPV6 address is
     * returned but the host kenel does not support IPV6.
     * If connect succeeds a break is made out of the loop.
     * Otherwise, when all the addresses have been tried , the loop also
     * terminates.Freeaddrinfo wrapper returns all the dynamic memory.
     */
	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sockfd < 0) continue;

        if( connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
        	break;
        Close(sockfd);

	
	}while( (res = res->ai_next) != NULL);

	if(res == NULL)
		printf("establish_tcp error, host: %s and server: %s", host, server);

	Freeaddrinfo(init);

	return sockfd;
}
