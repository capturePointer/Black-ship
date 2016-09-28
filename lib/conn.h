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

// define ipv4 connection
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
	uint8_t bufflen;
} conn_t;

// this type will be used for flag detection
// in order to know if we are using IPV4 or IPV6 conn
typedef enum ip_t {
	IPV4,
	IPV6,
} ip_t;


// conn_hints special type that will
// provide aditional information about what
// conn we want to establish.
// this can be used  with the conn_addr_setup family functions
typedef struct conn_hints{
	const char *host;
	const char *proto;
	struct addrinfo hints;
}conn_hints;

// alloc new connection
extern conn_t *conn_new(void);
extern void conn_free(conn_t *);
extern void conn_addr_setup(conn_t *, conn_hints);
#endif /*CONN_H*/
