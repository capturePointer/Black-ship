#include "net.h"
static char rtype(int type)
{
		int			types[9][2] = { {SOCK_STREAM,"TCP"}, 
									{SOCK_RAW, "RAW"},
									{SOCK_RDM, "RDM"},
									{SOCK_SEQPACKET, "D-LINK"},
									{SOCK_DCCP, "DCCP"},
									{SOCK_PACKET, "PACKET"},
									{SOCK_NONBLOCK, "NONBLOCK"},
									{SOCK_CLOEXEC, "EXEC"},
									{SOCK_DGRAM , "UDP"} };
		//hmm..
		//	for(int i=0;i<9;)

	return 'c';
}
char *
proto_ntop(int sockfd, const struct sockaddr *sa, socklen_t len)
{
	int				type;
	char			port[8];
	static char		message[128];

	switch(sa->sa_family)
	{
		case AF_INET:
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in*)sa;
			//adding address to "message"
			Inet_ntop(AF_INET, &ipv4->sin_addr,message,sizeof(message));
			
			//adding port to message
			//convers uint16_t from network byte order to host byte order
			if(ntohs(ipv4->sin_port)!=0) // if succeeds
			{
				snprintf(port,sizeof(port), ":%d",ntohs(ipv4->sin_port));
				strncat(message,port,sizeof(port));
			}
			
			Getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &type, &len);
			//add type to message	
			//strncat(message,type,sizeof(type));

			return message;
		}
		case AF_INET6:
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)sa;
			message[0] = '[';
			Inet_ntop(AF_INET6, &ipv6->sin6_addr,message+1,sizeof(message)-1);
			
			if(ntohs(ipv6->sin6_port) != 0)
			{
				snprintf(port,sizeof(port), ":]%d", ntohs(ipv6->sin6_port));
				strncat(message,port,sizeof(port));
				return message;
			}

			return message+1;
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
		case AF_ROUTE:
		{
		}
		case AF_KEY:
		{
		
		
		}
		default:
		{
			snprintf(message,sizeof(message),"proto_ntop: unknow AF_XXX: %d,len %d",sa->sa_family, len);
			return message;
		}
		return NULL;
	}

}

