#include "net.h"
/*
 * Our own standard I/O wrapper functions
 */
char *
Fgets(char *pointer,int n, FILE *stream)
{
	char *fpointer = fgets(pointer,n,stream);
	int file_error = ferror(stream);

	if( (fpointer== NULL) && (file_error) )
	{
		echo_error("Can't read line from stdin",true,errno);
		return NULL;
	}
	
	return fpointer;
}




