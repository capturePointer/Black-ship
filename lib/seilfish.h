#ifndef SEILFISH_H
#define SEILFISH_H
/*
 * My own custom barkeley C socket api header
 *
 *  Linux envirnoment
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h> /*time stamp*/
#include <time.h>
#include <stdint.h> /*unsinged uint32_t uint16_t types*/
#include <sys/socket.h> /*socket(),*/
#include <sys/un.h> /**/
#include <sys/types.h> /* size_t int8_t int16_t*/
#include <unistd.h> /* unxi standard library */
#include <arpa/inet.h> /*inet_pton*/
#include <errno.h> /*strerror()*/
#include <signal.h>/*catch signals*/
#include <sys/wait.h>
#include <sys/select.h> /*IO multiplexing*/
#include <poll.h>
#include <limits.h>
//INFTIM timeout value for pool , wait forever
#ifndef INFTIM
	#include <bsd/sys/poll.h>
#endif
#include <netinet/sctp.h> /*you must install lksctp_tools 
							in order to have this library*/


/*
 * Define simple variables
 */

//IPV4 structure adress
struct sockaddr_in server4_address;
struct sockaddr_in client4_address;
//IPV6 structe address
struct sockaddr_in6 server6_address;
struct sockaddr_in6 client6_address;
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
/*
 *  Prototype of our own functions
 *
 */
void    argc_length(int, int);
void    echo_error(const char*, bool, int);
void    prog_error(const char*, bool, int);
void 	error_addrinfo(const char *, bool, int);
/**
 * Miscellaneous
 */
int      max(int,int);
void    *Malloc(size_t);
void	 initz(void *,int);

ssize_t  s_write(int, const void*, size_t, bool);
ssize_t  s_read(int, void*, size_t, bool);
ssize_t	 readline(int,void*,size_t);

/**
 * Keyboard buffer input
 * functions
 */
void 	io_keyb(char *,int);

/* Wrapper base proto */
int			Socket(int , int , int );
void		Connect(int, const struct sockaddr*, socklen_t);
void		Inet_pton(int, const char*, void*);
void		Inet_ntop(int,const void*,char*,socklen_t);
void		Inet_aton(const char *, struct in_addr *);
char		*Fgets(char*, int, FILE*);
void		Fputs(const char*, FILE*);
ssize_t		Write(int, const void*, size_t);
ssize_t		Read(int, void*,size_t);
ssize_t		Sendto(int,const void*, size_t, int, const struct sockaddr*,
				   socklen_t);
ssize_t		Recvfrom(int, void *restrict , size_t, int,
				 struct sockaddr *restrict, socklen_t *restrict);
ssize_t		Send(int, const void*, size_t, int);
ssize_t		Recv(int, void*, size_t, int);
ssize_t		Sendmsg(int, const struct msghdr *, int);
ssize_t		Recvmsg(int, struct msghdr *, int);
void		Bind(int, const struct sockaddr*, socklen_t);
void		Listen(int, int);
int			Accept(int, struct sockaddr *restrict , socklen_t *restrict);
void		Close(int);
pid_t		Fork(void);
pid_t		Wait(int*);
pid_t		Waitpid(pid_t,int*,int);
Sigfunc		*Signal(int,Sigfunc*);
int			Select(int nfds, fd_set*, fd_set*, fd_set*, struct timeval*);
int			Poll(struct pollfd[],nfds_t,int);
void		Shutdown(int,int);
void		Setsockopt(int, int, int, const void*, socklen_t);
void		Getsockopt(int, int, int, void*, socklen_t*);
void		Getpeername(int, struct sockaddr*, socklen_t*);
void		Getsockname(int, struct sockaddr *restrict, socklen_t *restrict);
void		Getaddrinfo(const char *, const char *, const addrinfo *, 
						addrinfo **);
void		Freeaddrinfo(addrinfo *);
/*SCTP* wrapper based func*/
void		Sctp_bindx(int, struct sockaddr*, int, int);
void		Sctp_connectx(int, struct sockaddr*,int, sctp_assoc_t*);
void		Sctp_getpaddrs(int, sctp_assoc_t, struct sockaddr **);
void		Sctp_freepaddrs(struct sockaddr *);

void		Sctp_getladdrs(int, sctp_assoc_t, struct sockaddr **);
void		Sctp_freeladdrs(struct sockaddr *);

int			Sctp_sendmsg(int, const void *, size_t , struct sockaddr *,
                      socklen_t, uint32_t, uint32_t, uint16_t, uint32_t,
                      uint32_t);
int			Sctp_recvmsg(int, void*, size_t, struct sockaddr *, socklen_t *,
                      struct sctp_sndrcvinfo *,int *);

void		Sctp_opt_info(int, sctp_assoc_t, int, void *arg, socklen_t *);
int			Sctp_peeloff(int, sctp_assoc_t);
int			Sctp_send(int, const void *, size_t , const struct sctp_sndrcvinfo *
					  ,uint32_t);
/*SCTP handle functions*/
sctp_assoc_t sctp_address_to_associd(int ,struct sockaddr *, socklen_t);
int			sctp_get_number_streams_bsd(int, struct sockaddr *, socklen_t);
int			sctp_get_number_streams(int, struct sockaddr *, socklen_t,
									struct sctp_sndrcvinfo *);
void        sctp_set_number_streams(int, struct sctp_initmsg * ,int,int);
Sigfunc*	c_signal(int,Sigfunc*);
/* handler child signal base functions*/
void		handler_child_waitpid(int);
void		handler_child_wait(int);

char		*proto_ntop(int, const struct sockaddr*, socklen_t);
void		echo_name_socket(int);
void		tell_info_hosts(int, char **);
addrinfo	*host_serv(const char *, const char *,int, int );
/**
 *TCP libray headers
 */
int			host_tcp(const char *, const char *, socklen_t *);
int			establish_tcp(const char *, const char *);
#endif /* SEILFISH_H */
