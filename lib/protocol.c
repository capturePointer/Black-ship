#include "net.h"

char *proto_ntop(const struct sockaddr *sa, socklen_t)
{
	char			port[8];
	static char		message[128];

	switch(sa->sa_family)
	{
		case AF_INET:
		{
			if(inet_ntop(AF_INET, &ipv4->sin_addr,str,sizeof(str))== NULL)
				return NULL;
			if(ntohs(ipv4->sin_port)!=0)
			{
				snprintf(port,sizeof(port), ":%d",ntohs(ipv4->sin_port));
				strcat(message,port);
			}
			return message;
		}
		case AF_INET6:
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)sa;
			message[0] = '[';
			if(inet_ntop(AF_INET6, &ipv6->sin6_addr,message+1,sizeof(message)-1) == NULL)
				return NULL;
			if(ntohs(sin6->sin6_port) != 0)
			{
				snprintf(port,sizeof(port), ":]%d", ntohs(sin6->sin6_port));
				strcat(message,portstr);
				return message;
			}

			return message+1;
		}
		case AF_UNIX:
		{
			struct sockaddr_un *unix = (struct soackaddr_un *)sa;
			if(unix->sun_path[0] == 0)
				strcpy(message,"(no pathname bound)");
			else
				snprintf(message,sizeof(message),"%s",unix->sun_path);
			return message;
		}
	}

}

