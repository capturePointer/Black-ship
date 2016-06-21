#include "util.h"
#include "../err/err.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// uint16_t port_conv
// convert from a char* representation to port number
// ports are defined as uint16_t because are 16 byte wide.
// max value for a port should be UINT_MAX
uint16_t port_conv(const char *arg)
{
	unsigned long u = strtoul(arg, NULL, 10);
	// check if u is larger than 16 bytes or the parsing was invalid
	if ((errno == ERANGE) || (u > UINT_MAX)) {
		// ports are 16 byte wide
		err_new("[ERROR] Connot convert the number into a real port.", ERRLARGEVALUE, errno);
		return 0;
	}

	// it is safe is u is a value from 0 to UINT_MAX(65,535)
	return (uint16_t)u;
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
		err_new("[ERROR] Connot convert numbers into range ports.", ERRRANGEVALUE, 0);
		return;
	}
	*low = port_conv(t1);

	// in order to get the next token
	// and to continue with the same string
	// NULL is passed as first arg
	t2 = strtok(NULL, delim);
	if (t2 == NULL) {
		err_new("[Error] Connot convert numbers into range ports.", ERRRANGEVALUE, 0);
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
