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
		error_exit("Can't read line from stdin\n");

	return fpointer;
}




