#ifndef NET_H
	#define NET_H
/*
 * My own custom barkeley C socket api header
 *
 *	Linux envirnoment
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <netinet/in.h> /*sockaddr_in{},in_addr{} etc..*/
#include <sys/socket.h> /*socket(),*/
#include <sys/types.h> /* size_t int8_t int16_t*/
#include <unistd.h>
#include <arpa/inet.h> /*inet_pton*/
#include <errno.h> /*strerror()*/
#include <signal.h> /*catch signals*/
#include <sys/wait.h>
/*
 * Define simple variables
 */
//IPV4 structure adress
struct sockaddr_in server4_address;
struct sockaddr_in client4_address;

/*
 *	Define constant var
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
typedef void Sigfunc(int);
/**/

#ifndef LISTENQ
	#define LISTENQ 1024
#endif
/*
 *	Prototype of our own functions
 *
 */
void argc_length(int, int);
void echo_error(const char*, bool, int);
void prog_error(const char*, bool, int);

/* Wrapper base proto */
int	Socket(int , int , int );
void Connect(int, const struct sockaddr*, socklen_t);
void Inet_pton(int, const char*, void*);
char *Fgets(char*, int, FILE*);
void Fputs(const char*, FILE*);
size_t Write(int, const void*, size_t);
size_t Read(int, void*,size_t);
void Bind(int, const struct sockaddr*, socklen_t);
void Listen(int, int);
int Accept(int, struct sockaddr *restrict , socklen_t *restrict);
ssize_t s_write(int, const void*, size_t, bool);
ssize_t s_read(int, void*, size_t, bool);
void Close(int);
pid_t Fork(void);
Sigfunc *signal (int, Sigfunc*);
#endif /* NET_H */
