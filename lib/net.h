#ifndef NET_H
#define NET_H
/*
 * My own custom barkeley C socket api header
 *
 *  Linux envirnoment
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h> /*time stamp*/


#include <netinet/in.h> /*sockaddr_in{},in_addr{} etc..*/
#include <sys/socket.h> /*socket(),*/
#include <sys/types.h> /* size_t int8_t int16_t*/
#include <unistd.h> /* unxi standard library */
#include <arpa/inet.h> /*inet_pton*/
#include <errno.h> /*strerror()*/
#include <signal.h>/*catch signals*/
#include <sys/wait.h>
#include <sys/select.h> /*IO multiplexing*/
#include <poll.h>
#include <limits.h> // for OPEN_MAX
//INFTIM timeout value for pool , wait forever
#ifndef INFTIM
#include <sys/stropts.h>
#endif



/*
 * Define simple variables
 */

//IPV4 structure adress
struct sockaddr_in server4_address;
struct sockaddr_in client4_address;

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

#define SA struct sockaddr
/*Custom user based settings*/
#define PORT 9877
#define MAXLINE 4096
#define BUFFSIZE 8192


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
void     argc_length(int, int);
void     echo_error(const char*, bool, int);
void     prog_error(const char*, bool, int);
/**
 * Miscellaneous
 */
int      max(int,int);
void    *Malloc(size_t);


ssize_t  s_write(int, const void*, size_t, bool);
ssize_t  s_read(int, void*, size_t, bool);
ssize_t	 readline(int,void*,size_t);

/* Wrapper base proto */
int      Socket(int , int , int );
void     Connect(int, const struct sockaddr*, socklen_t);
void     Inet_pton(int, const char*, void*);
char    *Fgets(char*, int, FILE*);
void     Fputs(const char*, FILE*);
size_t   Write(int, const void*, size_t);
size_t   Read(int, void*,size_t);
void     Bind(int, const struct sockaddr*, socklen_t);
void     Listen(int, int);
int      Accept(int, struct sockaddr *restrict , socklen_t *restrict);
void     Close(int);
pid_t    Fork(void);
pid_t    Wait(int*);
pid_t    Waitpid(pid_t,int*,int);
Sigfunc *Signal(int,Sigfunc*);
int      Select(int nfds, fd_set*, fd_set*, fd_set*, struct timeval*);
void     Shutdown(int,int);
Sigfunc* c_signal(int,Sigfunc*);
/* handler child signal base functions*/
void     handler_child_waitpid(int);
void     handler_child_wait(int);


#endif /* NET_H */
