#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"

// info prints out to stdout the msg, file and line number
inline void info(const char *msg, const char *file, int line)
{
	fprintf(stdout, "%s %s %d\n", msg, file, line);
}

// info prints out to stdout the message, file and the line number
// after the message is writed to stdout, it exits the program
inline void infoee(const char *msg, const char *file, int line)
{
	fprintf(stderr, "%s %s %d\n", msg, file, line);
	exit(EXIT_FAILURE);
}
