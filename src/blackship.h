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

// thic cmd tool uses argp as it's parser mechanism for the
// command line arguments
#ifndef BLACKSHIP_H
#define BLACKSHIP_H

#include <netinet/in.h>
#include <sys/socket.h>

// define the sock_t type
typedef int sock_t;

// redefine to make it simpler to interface
typedef struct sockaddr_in addr4_t;
typedef struct sockaddr_in6 addr6_t;

// make is shorter for casts to this struct type
#define SA struct sockaddr

// define ipv4 connection
typedef struct conn4_t {
	// the underlying socket connection
	sock_t sock;
	// ptr to ipv4 socket addr struct
	// for ipv4 connections
	// this will hild all the info for special socket
	// operations like bind, liste, accept, conn, etc.
	addr4_t *addr;
	// the underlying buffer that the socket
	// will use for IO purposes
	unsigned char *buff;
} conn4_t;

// define ipv6 connection
typedef struct conn6_t {
	// the underlying socket connection
	sock_t sock;
	// ptr to ipv6 socket addr struct
	// for ipv6 connections
	// this will hild all the info for special socket
	// operations like bind, liste, accept, conn, etc.
	addr6_t *addr;
	// the underlying buffer that the socket
	// will use for IO purposes
	unsigned char *buff;
} conn6_t;

// ipv4 or ipv6 connections
typedef union conn_t {
	conn6_t *c6;
	conn4_t *c4;
}conn_t;

#endif /*BLACKSHIP_H*/
