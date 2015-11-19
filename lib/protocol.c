#include "sailfish.h"
/**
 * struct socket_types are all the socket type options
 * along with the normal recognizable name
 */
struct socket_types
{
	//socket type
	int sock_type;
	//Description
	const char *dscrp;
}socket_types[] = {
	{SOCK_STREAM,"TCP"}, 
	{SOCK_RAW, "RAW"},
	{SOCK_RDM, "RDM"},
	{SOCK_SEQPACKET, "D-LINK"},
	{SOCK_DCCP, "DCCP"},
	{SOCK_PACKET, "PACKET"},
	{SOCK_NONBLOCK, "NONBLOCK"},
	{SOCK_CLOEXEC, "EXEC"},
	{SOCK_DGRAM , "UDP"},
	{0,NULL}
};
/*
 * Search the hole big struct socket_type to find
 * the right type and return it's description
 */
static const char*
get_socket_type_descr(const int opt_type)
{
	const char *unknown_dscrp = "Uknown SOCK_XXX socket type";
	struct socket_types *ptr;
	for(ptr = socket_types; ptr->dscrp != NULL ; ptr++)
	{
		if(opt_type == ptr->sock_type)
			return ptr->dscrp;
	}
	return unknown_dscrp;
}
/**
 * proto_ntop supports only AF_INET,AF_INET6
 * and AF_UNIX aka AF_LOCAL
 */
char *
proto_ntop(int sockfd, const struct sockaddr *sa, socklen_t len)
{
	int				rtype;// getsockopt return type
	char			type[10];// the actual human readble tyepe
	char			port[10];
	static char		message[128];
	
	switch(sa->sa_family)
	{
		case AF_INET:
		{
			
			struct sockaddr_in *ipv4 = (struct sockaddr_in*)sa;
			message[0] ='[';
			//convert network to presentation and add address to "message"
			Inet_ntop(AF_INET, &ipv4->sin_addr,message+1,sizeof(message)-1);
			//convers uint16_t from network byte order to host byte order and add port.
			if(ntohs(ipv4->sin_port)!=0) // if succeeds
			{
				snprintf(port,sizeof(port), "] : %d,",ntohs(ipv4->sin_port));
				//add port to message
				strncat(message,port,sizeof(*port));
			}
			
			Getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &rtype, &len);
			
			snprintf(type,sizeof(type)," %s ",get_socket_type_descr(rtype));
			strncat(message,type,sizeof(*type));
			return message;
		}
		case AF_INET6:
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)sa;
			message[0] = '[';
			//covert netowrk to presentation and add adress to "message"
			Inet_ntop(AF_INET6, &ipv6->sin6_addr,message+1,sizeof(message)-1);
			//converts uint_16_t from network byte order to host byte order and add port.
			if(ntohs(ipv6->sin6_port) != 0)
			{
				snprintf(port,sizeof(port), "] : %d", ntohs(ipv6->sin6_port));
				//add port to message
				strncat(message,port,sizeof(*port));
			}
			Getsockopt(sockfd,SOL_SOCKET,SO_TYPE,&rtype,&len);
			snprintf(type,sizeof(type)," %s ",get_socket_type_descr(rtype));
			strncat(message,type,sizeof(*type));
			return message;
		}
		case AF_UNIX:
		{
			struct sockaddr_un *unl = (struct sockaddr_un *)sa;
			if(unl->sun_path[0] == 0)
				strncpy(message,"(no pathname bound)",19);
			else
				snprintf(message,sizeof(message),"%s",unl->sun_path);
			return message;
		}
		default:
		{
			snprintf(message,sizeof(message),"Unknow AF_XXX: %d,len %d",sa->sa_family, len);
			return message;
		}
	}
}

void
echo_name_socket(int sd)
{
	//we use sockaddr_storage because it can store both ipv4 or ipv6 
	struct sockaddr_storage addr;
	char ipstr[MAXLINE];
	int port;
	socklen_t len;
    char echo[MAXLINE];
	len = sizeof(addr);
	
	//echo = Malloc(MAXLINE *sizeof(char));
	Getpeername(sd, (SA*)&addr, &len);

	//deal with both ivp4, and ipv6
	switch(addr.ss_family)
	{
		case AF_INET:
		{
			struct sockaddr_in *s =(struct sockaddr_in *)&addr;
			port = htons(s->sin_port);
			Inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
			break;
		}
		case AF_INET6:
		{
			struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
			port = htons(s->sin6_port);
			Inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
			break;
		}
	}
    snprintf(echo, sizeof(echo),"Peer IP:PORT address: %s : %d\n",ipstr,port);
	printf("%s\n",echo);

}
/*
 * I know that gethostbyname
 * is deprecated and i should replace it
 * with getaddrinfo.
 * Below i will develop a side func for it
 */
void 
tell_info_hosts(int n, char **host_names)      // only for ipv-4
{
	char *ptr, **pptr;
	char str[BUFFSIZE];
	hostent *hptr;
 
		for(int i = 0; i<n; i++)
		{
			ptr = *host_names;
			host_names ++;
			if( (hptr = gethostbyname(ptr)) == NULL) {
				echo_error("Error gethostbyname\n",false,errno);
				printf("%s \n",hstrerror(h_errno));
				continue;
			}
            printf("Oficial hostname: %s\n",hptr->h_name);
			for(pptr = hptr->h_aliases; *pptr!=NULL; pptr++) {
                printf("\taliases: %s\n", *pptr);
			}
			for(pptr = hptr->h_addr_list ; *pptr!=NULL; pptr++) {
				Inet_ntop(hptr->h_addrtype, *pptr, str, sizeof str);
				printf("taddress: %s\n",str);
			}
			printf("\n\n");
			
		}
}


/**
 * get adress infomariton
 * Getaddrinfo Wrapper
 */

/*
 * struct addrinfo {
 *   int		ai_flags;		// AI_PASSIVE AI_CANONNAME
 *   int		ai_family;		//AF_XXX
 *   int		ai_socktype;	//SOCK_XXX
 *   int		ai_protocol;	// 0 or IPPROTO_XXX for ipv4 and ipv6
 *   socklen_t	ai_addrlen;		//length of ai_addr
 *   char		*ai_canonname;	// ptr to canonical name for host
 *   struct sockaddr  *ai_addr;	// ptr to socket address structure
 *   struct addrinfo  *ai_next;	 // ptr to next structure in liked list
 * };
 *
 * Despite the fact that getaddrinfo is better than the gethostbyname 
 * and getservbyname functions(it makes it easier to write protocol-independet
 * code; one function handles both the hostname and service;
 * and all the returned info is dynamically alocated, not
 * statically alocated), it is still not as easy to use as it could be.
 */
		
void 
Getaddrinfo(const char *hostname, const char *service, 
			const addrinfo *hints, addrinfo **result)
{
    int rgai = getaddrinfo(hostname, service, hints, result);
    if(rgai != 0) {
		printf("Getaddrinfo error : %s", gai_strerror(errno));
		exit(EXIT_FAILURE);
    }
}
/**
 * *res should point to the structure returned by geaddrinfo
 * All the strucres in the linked list are freed, along with the 
 * dynamic storage pointed to by those structure(example socket addres struct
 * and canonical hostnames).
 */
// keep it consistant
void
Freeaddrinfo(addrinfo *res)
{
	freeaddrinfo(res);
}

addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype)
{
	addrinfo hints, *res;
    // The function initializes a hints structure
    // calls wrapper Getaddrinfo and exit's if something is wrong
	memset(&hints, 0, sizeof(hints));
	// Setting this flag will always return the canon name
	hints.ai_flags = AI_CANONNAME;
	// Here we set the addr family, like AF_UNSPEC , AF_INET, AF_INET6 
	hints.ai_family = family;
    // We will set our protocol related socket like 0, SOCK_STREAM, SOCK_DRGRAM
	hints.ai_socktype = socktype;

	Getaddrinfo(host, serv, &hints, &res);
    return (res);   // return our pointer to first on linked list
}
