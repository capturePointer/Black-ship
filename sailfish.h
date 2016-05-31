#ifndef SAILFISH_H
#define SAILFISH_H

// #ifndef INFTIM
// 	#include <bsd/sys/poll.h>
// #endif
//

/*
 * Define simple variables
 */

//IPV4 structure adress
typedef struct sockaddr_in server4_address;
typedef struct sockaddr_in client4_address;
//IPV6 structe address
typedef struct sockaddr_in6 server6_address;
typedef struct sockaddr_in6 client6_address;
/*
 *  Define constant var
 */

//Colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"


#ifndef EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
    #define EXIT_FAILURE -1
#endif

#ifndef LISTENQ
    #define LISTENQ 1024
#endif
/**
 * We define a reasonable limit like in the X11/Xos.h form Xorg
 * It appears that OPEN_MAX is deprecated, at least on Linux systems.
 * The reason appears to be that the maximum number of file that
 * can be opened simultaneously is not fixed, so a macro
 * that expands to an integer literal is not a good way
 * to get that information.
 *
 *
 * If you want the current maximum number of files that can be opened,
 * take a look at the sysconf() function; on my system, sysconf(_SC_OPEN_MAX)
 * returns 1024.(The sysconf() man page refers to a symbol OPEN_MAX.
 * This is not a count, but a value recognized by sysconf().
 * And it's not defined on my system.)
 */
#ifndef OPEN_MAX
	#define OPEN_MAX 256
#endif


/**
 * for a better readability
 */
#define SA struct sockaddr
typedef struct hostent hostent;
typedef struct addrinfo addrinfo;

/*Custom user based settings*/
#define PORT 9877
#define MAXLINE 4096
#define BUFFSIZE 8192
/* SCTP FLAGS*/

//increment size of pdapi  when adding buf space
#define SCTP_PDAPI_INCR_SZ 65535
#define SCTP_PDAPI_NEED_MORE_THRESHOLD 1024
/**
 * For  the free bsd KAME implementation of SCTP
 * this default is set to 10 streams
 */
#define SERV_MAX_SCTP_STRM 10 //normal maxium streams
#define SERV_MORE_STRMS_SCTP 20 //larger number of streams

 /* we will replace this :
        void (* signal (int signo, void(*func) (int))) ) (int);
    with this :
        Sigfunc *signal(int signo, Sigfunc *func);
    in order to do this we must typedef smth
 */
typedef void	Sigfunc(int);
typedef struct  sigaction sig;
Sigfunc*	c_signal(int,Sigfunc*);

#endif /* SAILFISH_H */
