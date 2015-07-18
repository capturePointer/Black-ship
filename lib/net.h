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

/*
 * Define simple variables
 */
//IPV4 structure adress
struct sockaddr_in server4_address;
struct sockaddr_in client4_address;



/*
 *	Define constant var
 */

#ifndef EXIT_SUCCESS
	#define EXIT_SUCCESS 0 
#endif

#ifndef EXIT_FAILURE
	#define EXIT_FAILURE -1
#endif


#define SA struct sockaddr

/*Custom user based settings*/
#define PORT 9987
#define MAXLINE 4096
#define BUFFSIZE 8192


/*
 *	Prototype of our own functions
 *
 */
void argc_length(int ,int);
void error_exit(const char*);
void *error_system_exit(const char *message,void*);
/* Wrapper base proto */
int	Socket(int , int , int );
int Connect(int, const SA*,socklen_t);
void Inet_pton(int , const char*, void*);
char *Fgets(char* ,int,FILE*);
size_t Write(int, const void*,size_t);



#endif /* NET_H */
