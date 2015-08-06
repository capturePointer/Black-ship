#include "net.h"

int
max(int a, int b)
{
    return (a>b)?a:b;
}

void *Malloc(size_t size)
{
	void *ptr = malloc(size);
	if(ptr == NULL)
		prog_error("Unable to allocate memory error",true,errno);
	return ptr;
}
