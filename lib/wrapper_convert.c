#include "net.h"

/*
 * Presentation to network byte order
 */
void Inet_pton(int af, const char *src, void *dst)
{
	int inet = inet_pton(af,src,dst);

	if(inet == -1)
		error_exit("System convertion failed \n");
	else
		if(inet)
			error_exit("IP provided does not contain a valid representation of a valid network address\n");

}
