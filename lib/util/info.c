#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"

void info(const char *msg, const char *file, int line)
{
	fprintf(stdout, "%s %s %d", msg, file, line);
}

void infoe(const char *msg, const char *file, int line)
{
	fprintf(stderr, "%s %s %d", msg, file, line);
}

void infoee(const char *msg, const char *file, int line)
{
	fprintf(stderr, "%s %s %d", msg, file, line);
	exit(EXIT_FAILURE);
}
