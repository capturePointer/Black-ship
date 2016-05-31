#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include "sailfish.h"
#include "error.h"

// func for testing the number or argumnets in the comand line
// this is just for test purposes and it will be removed in the final
// release of the library
void sailfish_check_length(int argc,int expect)
{
	if(argc < expect) {
		printf(KBLU "::> "RESET "None argumnets passed.\n" );
		printf(KBLU "::> "RESET "Please pass more arguments.\n" );
		exit(EXIT_FAILURE);
	}
}

// display error
void sailfish_error(const char *message, bool display_errno,int errnoflag)
{
	if(display_errno)
		printf(KBLU "::> "KRED "%s : "RESET "%s.\n" ,message, strerror(errnoflag));
	else
		printf(KBLU "::> "KRED "%s.\n" RESET,message);
}

// critical error
// this func should be used just for critical errors
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

// error func for addrinfo
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
