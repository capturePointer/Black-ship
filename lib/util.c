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

int16_t port_conv(const char *arg)
{
	if (!filter_number(arg))
		return -1;

	long u = strtol(arg, NULL, 10);
	// check if u is larger than 16 bytes or the parsing was invalid
	if ((errno == ERANGE) || (u > UINT16_MAX) || (u < 0)) // ports are 16 byte wide
		return -1;

	return (int16_t)u;
}

int8_t port_conv_range(char *arg, uint16_t *low, uint16_t *high)
{
	if (!arg) {
		return -1;
	}

	char delim[2] = "-";
	char *t1, *t2;
	t1 = strtok(arg, delim);
	if (t1 == NULL)
		return -1;

	int16_t p = 0;

	if ((p = port_conv(t1)) == -1) {
		return -1;
	}

	*low = (uint16_t)p;

	// in order to get the next token
	// and to continue with the same string
	// NULL is passed as first arg
	t2 = strtok(NULL, delim);
	if (t2 == NULL) {
		return -1;
	}
	
	if ((p = port_conv(t2)) == -1) {
		return -1;
	}

	*high = (uint16_t)p;

	// swap values if their are in descent order
	if (*low > *high) {
		*low  = *low ^ *high;
		*high = *high ^ *low;
		*low  = *low ^ *high;
	}

	return 0;
}

bool valid_ip(const char *ip)
{
	if (!ip)
		return false;

	struct sockaddr_in in4;
	struct sockaddr_in6 in6;

	if (inet_pton(AF_INET, ip, &in4.sin_addr) ||
		inet_pton(AF_INET6, ip, &in6.sin6_addr))
		return true;

	return false;
}

static uint64_t seeds[2];

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


void port_seeds(void)
{
	// read from /dev/urandom 128 bytes
	if (!urandom_bytes(seeds, sizeof(seeds)))
		INFOEE("Could not find /udev/random to use as a source of entropy");

	// init the seeds
	pcg32_srandom(seeds[0], seeds[1]);
}

uint16_t port_random(void)
{
	return (uint16_t)pcg32_boundedrand(UINT16_MAX);
}

int64_t strconv(const char *n, uint8_t base)
{
	if (is_zero(n))
		return 0;


	int64_t result = strtoll(n, NULL, base);
	if (((errno == ERANGE) && (result == LLONG_MAX)) || 
			(errno == EINVAL) || (result == 0))
		return -1;
	
	return result;

}

static bool is_zero(const char *n) {
	if(strlen(n) == 1) {
		if (strncmp(n, "0", 1) == 0) 
			return true;
	}
	return false;
}

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

	if (sigaction(signo, &act, &oact) == -1)
		return SIG_ERR;

	return oact.sa_handler;
}
