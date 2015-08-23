#include "net.h"

char *
proto_ntop(const struct sockaddr *sa, socklen_t len)
{
	char			port[8];
	static char		message[128];

	//adding getsockopt( fd, SOL_SOCKET, SO_TYPE, &type, &length );
	// to get the socket type of the protocol...

	switch(sa->sa_family)
	{
		case AF_INET:
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in*)sa;
			Inet_ntop(AF_INET, &ipv4->sin_addr,message,sizeof(message));
			//convers uint16_t from network byte order to host byte order
			if(ntohs(ipv4->sin_port)!=0) // if succeeds
			{
				snprintf(port,sizeof(port), ":%d",ntohs(ipv4->sin_port));
				strncat(message,port,sizeof(port));
			}
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

