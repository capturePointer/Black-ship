#include "util.h"

/**
 * This functions reacts like the wrapper Fgets but without the
 * containing the ENTER/newline char '\n'
 */
void sailfish_io_keyb(char *buff, int size)
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

inline int sailfish_max(int a, int b) { return (a>b)?a:b; }

void *sailfish_malloc(size_t size)
{
	void *ptr = malloc(size);
	if(ptr == NULL)
		prog_error("Unable to allocate memory error",true,errno);
	return ptr;
}
