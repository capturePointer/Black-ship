#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"

inline void info(const char *msg, const char *file, int line)
{
	fprintf(stdout, "%s %s %d\n", msg, file, line);
}

inline void infoe(const char *msg, const char *file, int line)
{
	fprintf(stderr, "%s %s %d\n", msg, file, line);
}

inline void infoee(const char *msg, const char *file, int line)
{
	fprintf(stderr, "%s %s %d\n", msg, file, line);
	exit(EXIT_FAILURE);
}
