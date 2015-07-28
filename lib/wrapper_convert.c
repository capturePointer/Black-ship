#include "net.h"
/*
 * Presentation to network byte order
 */
void Inet_pton(int af, const char *src, void *dst)
{
	int inet = inet_pton(af,src,dst);

	if(inet == -1)
		prog_error("Ip conversion error",true,errno);
	else
		if(inet == 0)
			prog_error("Invalid ip address",false,errno);
}
