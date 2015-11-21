#include "sailfish.h"
/*
 * Presentation to network byte order
 */
void 
Inet_pton(int af, const char *src, void *dst)
{
	int inet = inet_pton(af,src,dst);

	if(inet == -1)
		prog_error("Ip conversion error",true,errno);
	else
		if(inet == 0)
			prog_error("Invalid ip address",false,errno);
}
/**
 * Network to presentation
 */
const char *
Inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
	const char *inet = inet_ntop(af,src,dst,size);
	if(inet == NULL)
		prog_error("Network to presentaion converison error",true,errno);
	return inet;
}




/**
 * Convert Ip adress form dots and number string to a struct in_addr and back
 */

// NOTE THAT THIS METHOD IS DEPRECATED ! Use inet_pton() or inet_ntop instead

/**
 * This functions are deprecated because they don't handle IPV6 ! They are included here
 * because they can still be found in the wild
 */

void
Inet_aton(const char *cp, struct in_addr *inp){
   int n;
   if( (n = inet_aton(cp, inp)) == 0){
		prog_error("Inet_aton invalid address", true, errno);
   }
}
