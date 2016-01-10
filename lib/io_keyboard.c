#include "sailfish.h"

/**
 * This functions reacts like the wrapper Fgets but without the
 * containing the ENTER/newline char '\n'
 */
void 
io_keyb(char *buff, int size)
{
	char *rfget = fgets(buff, size, stdin);
	int ferr = ferror(stdin);
    size_t n;
	if ((rfget == NULL ) && (ferr)) {
	     printf("io_keyb error");
	     exit(EXIT_FAILURE);
	}
	
	n = strlen(buff);
	buff[n - 1] = '\0';
}
