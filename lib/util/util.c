#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "../err/err.h"
#include "util.h"

// filter_number()
// filters all character in the block of mem and it tests
// if we have in the block only digits and return true
// if we have letters or other simbols just return false
// this function exptects that the string is NULL terminated or it will fail
bool filter_number(const char *arg)
{
	const char *p = arg;
	for (; true; p++) {
		if (*p == '\0')
			return true;
		// if it's not in this interval
		if (!(*p >= 0x30 && (*p <= 0x39)))
			return false;
	}
}

// uint16_t port_conv
// convert from a char* representation to port number
// ports are defined as uint16_t because are 16 byte wide.
// max value for a port should be UINT_MAX
uint16_t port_conv(const char *arg)
{
	if (!filter_number(arg))
		goto err;

	long u = strtol(arg, NULL, 10);
	// check if u is larger than 16 bytes or the parsing was invalid
	if ((errno == ERANGE) || (u > UINT16_MAX) || (u < 0)) // ports are 16 byte wide
		goto err;

	// it is safe is u is a value from 0 to UINT_MAX(65,535)
	return (uint16_t)u;

err:
	err_new("[ERROR] Connot convert the number into a real port.", ERRCONVPORT, errno);
	return 0;
}

// uint16_t port_conv_range()
// converts a string reperesentatio of two numbers x-y delim with "-"
// the function wraps port_conv for every token
void port_conv_range(char *arg, uint16_t *low, uint16_t *high)
{
	char delim[2] = "-";
	char *t1, *t2;
	t1 = strtok(arg, delim);
	if (t1 == NULL) {
		err_new("[ERROR] Connot convert numbers into range ports.", ERRCONVPORT, 0);
		return;
	}
	*low = port_conv(t1);

	// in order to get the next token
	// and to continue with the same string
	// NULL is passed as first arg
	t2 = strtok(NULL, delim);
	if (t2 == NULL) {
		err_new("[Error] Connot convert numbers into range ports.", ERRCONVPORT, 0);
		return;
	}
	*high = port_conv(t2);

	// swap values if their are in descent order
	if (*low > *high) {
		*low  = *low ^ *high;
		*high = *high ^ *low;
		*low  = *low ^ *high;
	}
}

// valid_ip()
// test if it's a valid ip(ipv4,ipv6) and return true
// if it's not a valid ip return false
bool valid_ip(const char *ip)
{
	struct sockaddr_in in4;
	struct sockaddr_in6 in6;

	if (inet_pton(AF_INET, ip, &in4.sin_addr))
		return true;
	if (inet_pton(AF_INET6, ip, &in6.sin6_addr))
		return true;

	return false;
}
