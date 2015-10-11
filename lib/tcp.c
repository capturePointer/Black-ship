#include "seilfish.h"
/**
 * host_tcp performs the normal tcp
 * server steps:
 * create a tcp socket, bind the server's well known port
 * and allow incoming connection requests to be accepted
 *
 */
int 
host_tcp(const char *hostname, const char *service, socklen_t *addrlenptr)
{
	int listenfd, n;
	const int on = 1;
	struct addrinfo hints, *res, *init;
	bool flag_getaddr = false,
		 flag_bind_error = false;

	memset(&hints, 0,sizeof(hints));
	/**
	* We initialize an addrinfo structure with our
	* hints: AI_PASSIVE, since this function is for server,
	* AF_UNSPEC, for the address family and SOCK_STREAM.
	* If a hostname is not specified(which is common for a server
	* that wants to bind the wildcard address
	* the AI_PASSIVE and AF_UNSPEC hints will cause two socket address
	* structures to be returned: the first for IPv6 and the next for
	* IPv4(assuming a dual-stack host)
	*/
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if( ( n = getaddrinfo(hostname,service, &hints, &res)) != 0) {
		printf("host_tcp error for %s %s %s\n", hostname, service,
												gai_strerror(n));
		flag_getaddr = true;
	}

    //shallow copy
	init = res;
	
	/**
	* The socket and bind functions are called.
	* If either call fails, we just ignore the addrinfo struct
	* and move on to the next one.We always set the SO_REUSEADDR
	* socket option for a TCP SERVER
	*/
	do {
		listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		
		if(listenfd < 0)
			continue;

		Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		if(bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
			 break;

		Close(listenfd);
	
	}
	while( (res = res->ai_next) != NULL);
	
	if(res == NULL) {
		printf("Can't host a tcp process server for %s %s\n",hostname, service);
		flag_bind_error = true;
	}
	
    Listen(listenfd, LISTENQ);
	/**
	*  If the addrlenp argument is non-null, we return the size of the protocol
	*  addresses through this pointer.This allows the caller to allocate memory
	*  for a socket address structure to obtain the client's protocol address
	*  from accept
	*/
	
	if(addrlenptr) {
		*addrlenptr = res->ai_addrlen;// store it
	}
	
	Freeaddrinfo(init);

	if(flag_getaddr  || flag_bind_error) {
	   exit(EXIT_FAILURE);
	}

	return listenfd;
}
/**
 * The function(and our other functions that prove a simple interface
 * to getaddrinfo in the following sections), terminates if either
 * gataddrinfo fails or no call to connect succeeds.The only
 * return is upon success.It would be ahrd to return an error code
 * (one of the EAI_XXX constants) without adding another argument.
 */
int 
establish_tcp(const char *hostname, const char *service)
{
	int sockfd,n;
	addrinfo hints, *res, *init;
	bool flag_getaddr = false,
		 flag_conn_error = false;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype= SOCK_STREAM;
	/**
	 * Getaddrinfo is called one and we specify the addr
	 * family as AF_UNSPEC and the socket type as SOCK_STREAM
	 */
	if( (n = getaddrinfo(hostname, service, &hints, &res)) != 0){
		printf("establish_tcp nothing to return ,%s, %s: %s\n",hostname, service, gai_strerror(n));
		flag_getaddr = true;
	}

	/**
	 * shallow copy
	 * make a save var for res value
	 */
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
		if(sockfd < 0)
			continue;
		/**
		* If we have a SUCCESSFULL connection
		* stop the execution
		*/
		if( connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;
		/**
		* If the execution dosen't stoped
		* just close the failed socket
		*/
		Close(sockfd);

	}
	while( (res = res->ai_next) != NULL);

	if(res == NULL) {
		printf("Can't esablish TCP connection with host: %s and server type: %s", hostname, service);
		flag_conn_error = true;
	}
	/**
	* If everything works or not
	* in the end free memory of our save variable
	*/
	Freeaddrinfo(init);

	/**
	 * If something bad had happened
	 * exit imediatly terminating the hole
	 * program
	 */
	if(flag_getaddr || flag_conn_error) {
		exit(EXIT_FAILURE);
	}

	/**
	 * If everything worked it will return the 
	 * connected socket
	 */
	return sockfd;
}
