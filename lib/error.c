#include "net.h"

void 
argc_length(int argc,int expect)
{
	if(argc <expect)
	{
		printf("None argumnets passed.\nPlease pass more arguments\n)";
		
		exit(EXIT_FAILURE);
	}
	
}

