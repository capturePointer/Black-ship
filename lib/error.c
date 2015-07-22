#include "net.h"

void 
argc_length(int argc,int expect)
{
	if(argc < expect)
	{
		printf(KBLU "::> "RESET "None argumnets passed.\n" );
		printf(KBLU "::> "RESET "Please pass more arguments.\n" );
		exit(EXIT_FAILURE);
	}
}
void
echo_error(const char *message, bool display_errno,int errnoflag)
{
	if(display_errno)
		printf(KBLU "::> "KRED "%s : "RESET "%s.\n" ,message,strerror(errnoflag));
	else
		printf(KBLU "::> "KRED "%s.\n" RESET,message);
}
void
prog_error(const char *message,bool display_errno,int errnoflag)
{
	if(display_errno)
	{
		printf(KBLU "::> "KRED "%s : "RESET"%s.\n" ,message,strerror(errnoflag));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf(KBLU "::> "KRED "%s.\n" RESET ,message);
		exit(EXIT_FAILURE);
	}
}