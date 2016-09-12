// Copyright [2016] [hoenir]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
#include <linux/random.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "err.h"
#include "info.h"
#include "util.h"
#include "mem.h"
#include "pcg.h"

// filter_number filters all character in the block of mem and it tests
// if we have in the block only digits and return true
// if we have letters or other simbols just return false
// this function exptects that the string is NULL terminated or it will fail
bool filter_number(const char *arg)
{
	const char *p = arg;
	for (; *p !='\0'; p++)
		
		// if it's not in this interval
		if (!(*p >= 0x30 && (*p <= 0x39)))
			return false;

	return true;
}

// port_conv convert from a char* representation to port number
// ports are defined as uint16_t because are 16 byte wide.
// max value for a port should be UINT_MAX
uint16_t port_conv(const char *arg)
{
	if (!filter_number(arg))
		goto err;

	long u = strtol(arg, NULL, 10);
	// check if u is larger than 16 bytes or the parsing was invalid
	if ((errno == ERANGE) || (u > UINT16_MAX) || (u < 0))					 // ports are 16 byte wide
		goto err;

	// it is safe is u is a value from 0 to UINT_MAX(65,535)
	return (uint16_t)u;

err:
	err_new("[ERROR] Connot convert the number into a real port.", ERRCONVPORT, errno);
	return 0;
}

// port_conv_range converts a string reperesentatio of two numbers x-y delim with "-"
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

// xsprintf safe wrapper on sprintf
// this function pre calculates how much bytes needs to alloc
// in order to store into that buffer.
// if the buffer is still to small it will print out and error
// and terminate the program 
// the func will return a ptr to that newly created message
char *xsprintf(const char *fmt, ...) {
	va_list args;
	int n;
	char *buff;
	
	// first we must count the number of bytes we need
	// to alloc in order to store our buffer.
	va_start(args, fmt);
	n = vsnprintf(NULL,0, fmt, args);
	va_end(args);

	// alloc the buffer
	buff = xzmalloc(sizeof(char)*(unsigned long)n);
	va_start(args, fmt);
	vsnprintf(buff, sizeof(char)*(unsigned long)n, fmt, args);
	va_end(args);

	return buff;
}

// valid_ip test if it's a valid ip(ipv4,ipv6) and return true
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

// bool urandom_bytes
// Use /dev/urandom to get some entropy bytes for seeding purposes.
// 
// If reading /dev/urandom fails(which ought to never happen), it returns
// false, otherwise it return true.
bool urandom_bytes(void *dest, size_t size)
{
	if (size == 0)
		return false;

	int fd = open("/dev/urandom", O_RDONLY);
	if(!fd)
		return false;

	ssize_t sz = read(fd, dest, size);
	if(sz < (ssize_t)size)
		return false;
	return close(fd) == 0;
}

static uint64_t seeds[2];

// void port_seeds(void)
// seeds two 64 byes numbers, the state and the initseq
// it returns ERRENTROPY if the we can't read form the source of entropy
// the source of entropy that this functions uses is /dev/urandom.
void port_seeds(void) {
	// read from /dev/urandom 128 bytes
	if(!urandom_bytes(seeds, sizeof(seeds))) {
		err_new("Can't read from /udev/random", ERRENTROPY, 0);
		return;
	}
	
	// init the seeds
	pcg32_srandom(seeds[0], seeds[1]);
}

// uint16_t port_random(void)
// generates a valid port number from 1 to UINT16_MAX
uint16_t port_random(void)
{
	return (uint16_t)pcg32_boundedrand(UINT16_MAX);
}
