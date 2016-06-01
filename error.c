#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "sail.h"

// infoe displays the error in a nicer way
Sail infoe(const char *message, bool display_errno, int errnoflag)
{
	if (display_errno)
		printf(KBLU "::> " KRED "%s : " RESET "%s.\n", message,
		       strerror(errnoflag));
	else
		printf(KBLU "::> " KRED "%s.\n" RESET, message);
}

// infoe_exit displays the error and terminates the process
Sail infoe_exit(const char *message, bool display_errno, int errnoflag)
{
	if (display_errno) {
		printf(KBLU "::> " KRED "%s : " RESET "%s.\n", message,
		       strerror(errnoflag));
		exit(EXIT_FAILURE);
	}
	else {
		printf(KBLU "::> " KRED "%s.\n" RESET, message);
		exit(EXIT_FAILURE);
	}
}

// infoe_exit_addrinfo displays the error and terminates the process
// type of error comming from addrinfo
Sail infoe_exit_addrinfo(const char *message, bool display_errno, int errnoflag)
{
	if (display_errno) {
		printf(KBLU "::>" KRED ":%s :" RESET "%s.\n", message,
		       gai_strerror(errnoflag));
		exit(EXIT_FAILURE);
	}
	else {
		printf(KBLU "::> " KRED "%s.\n" RESET, message);
		exit(EXIT_FAILURE);
	}
}
