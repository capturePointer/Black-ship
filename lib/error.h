#ifndef ERROR_H
#define ERROR_H

extern void sailfish_check_length(int, int);
extern void sailfish_error(const char *, bool, int);
extern void sailfish_crt_error(const char *, bool, int);
extern void sailfish_error_addrinfo(const char *, bool, int);

#endif
