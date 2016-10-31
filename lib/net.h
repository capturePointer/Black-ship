// Copyright [2016] [hoenir]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef CONN_H
#define CONN_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netdb.h>

// define the sock_t type
typedef int sock_t;

// make is shorter for casts to this type
#define SA struct sockaddr

typedef struct conn_t {
	// the underlying socket connection
	sock_t sock;
	// ptr to ipv4/ipv6 socket addr struct
	// for ipv4/ipv6 connections
	// this will hold all the info for special socket
	// operations like bind, listen, accept, conn, etc.
	struct sockaddr_storage *addr;
	// the underlying buffer that the socket
	// will use for IO purposes
	uint8_t *buff;
	uint16_t bufflen; // max value 1<<16
} conn_t;

#define MAXBUFF 1<<16

// IP_SPEC
// helper macro for deciding upon which AF_* to choose
#define IP_SPEC(ipv) ((ipv == IPV4) ? AF_INET : AF_INET6)

/**
 * conn_new
 *
 * construct a new empty connection
 */
extern conn_t *conn_new(void);

/**
 * conn_free
 *
 * @conn - the connection you wish to free
 *
 * free up the connection @conn
 */
extern void conn_free(conn_t *conn);

/**
 * conn_buff_new
 *
 * @conn - conneciton you wish to set up the internal buffer
 * @sz - the size of the buffer you wish to set up
 *
 * alloc the internal buffer of the connection @conn suplied
 */
extern void conn_buff_new(conn_t *conn, uint16_t sz);

/*
 * conn_buff_free
 *
 * @conn - the intenral buffer of the connection you wish to free
 *
 * free up the internal buffer of the conneciton
 */
extern void conn_buff_free(conn_t *conn);

#endif /*CONN_H*/
