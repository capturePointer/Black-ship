#ifndef IOSOCKET_H
#define IOSOCKET_H

#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>


extern ssize_t sail_write(int, const void*, size_t, bool);
extern ssize_t sail_readline(int, void*, size_t);
extern ssize_t sail_read(int, void*, size_t, bool);

#endif
