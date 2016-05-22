#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include "sailfish.h"
#include "error.h"

void sailfish_check_length(int argc,int expect)
{
	if(argc < expect) {
		printf(KBLU "::> "RESET "None argumnets passed.\n" );
		printf(KBLU "::> "RESET "Please pass more arguments.\n" );
		exit(EXIT_FAILURE);
	}
}

void sailfish_error(const char *message, bool display_errno,int errnoflag)
{
	if(display_errno)
		printf(KBLU "::> "KRED "%s : "RESET "%s.\n" ,message,strerror(errnoflag));
	else
		printf(KBLU "::> "KRED "%s.\n" RESET,message);
}

void sailfish_crt_error(const char *message,bool display_errno,int errnoflag)
{
	if(display_errno) {
        printf(KBLU "::> "KRED "%s : "RESET"%s.\n" ,message,strerror(errnoflag));
		exit(EXIT_FAILURE);
	} else {
	    printf(KBLU "::> "KRED "%s.\n" RESET ,message);
		exit(EXIT_FAILURE);
	}
}
/**
 *  Error handler for addrinfo structure
 *  gai_error(errnoflag);
 *
 */
void sailfish_error_addrinfo(const char *message, bool display_errno, int errnoflag)
{
	if(display_errno) {
		printf(KBLU "::>" KRED ":%s :" RESET "%s.\n",message, gai_strerror(errnoflag));
		exit(EXIT_FAILURE);
	} else {
		printf(KBLU "::> "KRED "%s.\n"  RESET, message);
		exit(EXIT_FAILURE);
   }
}
