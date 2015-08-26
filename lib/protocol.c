#include "net.h"
/**
 * struct socket_types are all the socket type options
 * along with the normal recognizable name
 */
struct socket_types
{
	//socket type
	int sock_type;
	//Description
	char *dscrp;
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
static char*
get_socket_type_descr(const int opt_type)
{
	char *unknown_dscrp= "Uknown SOCK_XXX socket type";
	struct socket_types *ptr;
	for(ptr = socket_types;ptr->dscrp != NULL ; ptr++)
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
	int				rtype;//getsockopt return type
	char			type[10];//the actual human readble tyepe
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
				strncat(message,port,sizeof(port));
			}
			
			Getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &rtype, &len);
			
			snprintf(type,sizeof(type)," %s ",get_socket_type_descr(rtype));
			strncat(message,type,sizeof(type));
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
				strncat(message,port,sizeof(port));
			}
			Getsockopt(sockfd,SOL_SOCKET,SO_TYPE,&rtype,&len);
			snprintf(type,sizeof(type)," %s ",get_socket_type_descr(rtype));
			strncat(message,type,sizeof(type));
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
