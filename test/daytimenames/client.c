#include "lib/seilfish.h"

/**
 * Definition of our data types
 */
int		sockfd,n;
char	recvline[MAXLINE+1 ];
struct	in_addr **pptr;
struct	in_addr *inetaddrp[2];
struct	in_addr inetaddr; //non pointer
struct	hostent *hp;
struct	servent *sp;

/**
 * For debugging reseason 
 *
 *
 *  struct hostent {
 *	  char *h_name;  // oficial name of host
 *	  char **h_aliases; // aliast list
 *	  int h_addrtype;   // host address type // terminated by a null ptr
 *	  int h_length;     // liength of address
 *	  char **h_addr_list;   // list of addresses
 *  };
 *  #define h_addr h_addr_list[0] // for backwrad compatibility
 *
 *  struct servent {
 *    char *s_name;		  // oficial service name
 *    char **s_aliases;  // alias list , null terminated
 *    int	s_port;      // port number
 *    char *s_proto;     // protocol to use
 *	};
 *
 *	struct in_addr {
 *	  unsigned long s_addr; //load with inet_aton() // but this is quite deprecated 
 *	}
 */

int main(int argc, char *argv[])
{

	/**
	 * The first command line argumnets is a ostname,
	 * which we pas and argument to gethostbyname(),
	 * and hte second is a service name, which we pass as an
	 * argument to getservbyname.Our code assumes TCP, and that is
	 * what we use as the second argument to getservbyname.If 
	 * gethostbyname fails to look up the name, we try using Inet_aton
	 * wrapper function to see if the argument was an ASCI_FORMAT addr.
	 * If it was , we construct a single element list consists of the coresponding
	 * addr;
	 */
	argc_length(argc, 3); // test if the argc is min than 3.
	
	// test if we find the host and if we don't find we ..
	if( (hp = gethostbyname(argv[1])) == NULL) {
		Inet_aton(argv[1], &inetaddr); // if this will fail it will echo and terminat the process instantly
	    // if it's ok... continue
		inetaddrp [0] = &inetaddr;
		inetaddrp [1] = NULL; // the break point// finish
		pptr = inetaddrp; // direct pptr to point to this struct
	}else {
		//super cast
		pptr = (struct in_addr **)hp->h_addr_list;// cat this array of pointer of char into struct in_addr
	}
	if( (sp = getservbyname ( argv[2], "tcp")) == NULL){
		echo_error("getservbyname error",false,errno);
		exit(EXIT_FAILURE);
	}  // if thiw will fail
	
	/**
	 * We now code the calls to socket and connect in a loop that is executed for every
	 * server unit a connect succeeds or the list of IP addr is exhausted.After calling socket,
	 * we fill internet socket addr structure with the IP addr and port of the server.While we could move
	 * call to bzero, memset or initz and the subsequent twho assignments out of the loop, for
	 * efficiency, the code is easier and read as shown.Establishing the connection with the server is rarely a
	 * performance bottleneck for network client.
	 */
	for( ; *pptr != NULL; pptr++) {
		sockfd = Socket(AF_INET, SOCK_STREAM, 0); // create a simple tcp socket
		initz(&server4_address, 0);
		
		server4_address.sin_family = AF_INET;
		server4_address.sin_port   = sp->s_port;
		// copy all the info from our pptr that stores all the h_addr_list
		memcpy(&server4_address.sin_addr, *pptr, sizeof(struct in_addr));
		printf("trying %s\n",proto_ntop(sockfd, (SA*)&server4_address, sizeof(server4_address)) );
		
		// connect is called and if it succeds, break terminates the loop.
		// If the conneciton established fails, we print an error and close the socket.
		// Recall That a descriptor that fails a call to connect 
		// may close and is no longer usable
		if(connect(sockfd, (SA*)&server4_address, sizeof(server4_address) == 0))
	    	break;
		
		printf("connect error");
		close(sockfd);
	}
	// If the loop terminates ecause no call to connect
	// succeeded, the program terminates
	if( *pptr == NULL){
		printf("Unable to connect\n");
		exit(EXIT_FAILURE);
	}
	/**
	* Other wise we read the server,s respose , terminating when the
	* server closes the connection
	*/
	while( ( n = Read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		Fputs(recvline, stdout);
	}
	exit(EXIT_SUCCESS);
}
