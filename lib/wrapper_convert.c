#include "net.h"
/*
 * Presentation to network byte order
 */
void Inet_pton(int af, const char *src, void *dst)
{
	int inet = inet_pton(af,src,dst);

	if(inet == -1)
		printf("System convertion failed \n");
	else
		if(inet)
			printf("IP provided does not contain a valid representation of a valid network address\n");

}
