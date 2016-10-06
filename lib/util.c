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
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "err.h"
#include "info.h"
#include "mem.h"
#include "pcg.h"
#include "util.h"


static bool is_zero(const char *);

/*
 * filter_number 
 * @arg - string null terminated
 * 
 * filters all character and test if we have only digits 
 *
 * Return:
 *	if we have only digits return true
 *	if we have letters or other simbols just return false
 */
bool filter_number(const char *arg)
{
	if (!arg) {
		return false;
	}

	const char *p = arg;
	for (; *p != '\0'; p++)

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
	err_new("Connot convert the number into a real port.", ERRCONVPORT, errno);
	return 0;
}

// port_conv_range converts a string reperesentatio of two numbers x-y delim with "-"
// the function wraps port_conv for every token
void port_conv_range(char *arg, uint16_t *low, uint16_t *high)
{
	if (!arg) {
		err_new("Connot convert null arg into range ports.", ERRCONVPORT, 0);
		return;
	}

	char delim[2] = "-";
	char *t1, *t2;
	t1 = strtok(arg, delim);
	if (t1 == NULL) {
		err_new("Connot convert first number into range ports.", ERRCONVPORT, 0);
		return;
	}
	*low = port_conv(t1);

	// in order to get the next token
	// and to continue with the same string
	// NULL is passed as first arg
	t2 = strtok(NULL, delim);
	if (t2 == NULL) {
		err_new("Connot convert second number into range ports.", ERRCONVPORT, 0);
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
char *xsprintf(const char *fmt, ...)
{
	va_list args;
	int n;
	char *buff;

	// first we must count the number of bytes we need
	// to alloc in order to store our buffer.
	va_start(args, fmt);
	n = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	// alloc the buffer
	buff = xzmalloc(sizeof(char) * (unsigned long)n);
	va_start(args, fmt);
	vsnprintf(buff, sizeof(char) * (unsigned long)n, fmt, args);
	va_end(args);

	return buff;
}

// valid_ip test if it's a valid ip(ipv4,ipv6) and return true
// if it's not a valid ip return false
bool valid_ip(const char *ip)
{
	if (!ip)
		return false;

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
	if ((!dest) || (!size))
		return false;

	int fd = open("/dev/urandom", O_RDONLY);
	if (!fd)
		return false;

	ssize_t sz = read(fd, dest, size);
	if (sz < (ssize_t)size)
		return false;
	return close(fd) == 0;
}

static uint64_t seeds[2];

// void port_seeds(void)
// seeds two 64 byes numbers, the state and the initseq
// it returns ERRENTROPY if the we can't read form the source of entropy
// the source of entropy that this functions uses is /dev/urandom.
void port_seeds(void)
{
	// read from /dev/urandom 128 bytes
	if (!urandom_bytes(seeds, sizeof(seeds))) {
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

/* 
 * strconv
 * convert the string n to a specific format byte
 * @n	 - the string
 * @base - the base number
 *
 * Side note: very important that n ends with '\0' or this will have very bad side effects
 *
 * Return:
 *	if the conversion fails due to invalid @base or invalid @n string it will
 *	return 0 and ERRCONV into the err circular list.
 *	if the conversion succeeds it will return a valid number
 */
uint64_t strconv(const char *n, uint8_t base)
{
	if (is_zero(n))
		return 0;

	uint64_t result = 0;

	result = strtoull(n, NULL, base);
	if (((errno == ERANGE) && (result == ULLONG_MAX)) || (errno == EINVAL) ||(result == 0))
		goto fail;
	
	return result;

fail:
	err_new("Invalid string or invalid base, the string must contain only numbers", ERRCONV, 0);
	return 0;
}

static bool is_zero(const char *n) {
	if(strlen(n) == 1) {
		if (strncmp(n, "0", 1) == 0) 
			return true;
	}
	return false;
}

/*
 * treat_signal
 * friendly wrapper around sigaction
 *
 * function allows the calling process to specify the action 
 * to be associated with a specific signal.
 *
 * @signo - specifies the signal values define in <signal.h>
 * @fn - pointer to a signal-catching function or one of the macros
 * SIG_IGN or SIG_DFL.
 *
 */
sigfn treat_signal(int signo, sigfn fn)
{
	struct sigaction act, oact;
	memset(&act, 0, sizeof(act));
	memset(&oact, 0, sizeof(oact));

	act.sa_handler = fn;
	// set sa_mask to be emptyset, meaning no aditional
	// signals are blocked other than the signal being caught
	sigemptyset(&act.sa_mask);

	// if a system call is interrupted by this signal
	// will be automatically restarted by the kernel.
	if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
		// when generating SIGALRM normally places a timeout on an
		// I/O operation, in which case, we want the blocked system call to be
		// interrupted by this signal.
		// Some older system notably SunOS 4.x, automatically restart an interrupted system call
		// by default and then define the complement of this flag as SA_INTERRUPT.
		act.sa_flags |= SA_INTERRUPT; 
#endif
	} else {
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if (!sigaction(signo, &act, &oact))
		return SIG_ERR;

	return oact.sa_handler;
}
