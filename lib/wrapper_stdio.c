#include "seilfish.h"
/*
 * Our own standard I/O wrapper functions
 */
char *
Fgets(char *pointer,int n, FILE *stream)
{
	char *fpointer = fgets(pointer,n,stream);
	int file_error = ferror(stream);

	if( (fpointer == NULL) && (file_error) )
	{
        echo_error("Can't read line from stdin",true,errno);
		return NULL;
	}

	return fpointer;
}
void
Fputs(const char *pointer, FILE *stream)
{
	if(fputs(pointer,stream) == EOF )
	 	prog_error("Can't output line from buffer given",true,errno);
}
