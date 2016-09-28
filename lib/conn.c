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
#include <string.h>
#include <sys/types.h>

#include "conn.h"
#include "err.h"
#include "info.h"
#include "mem.h"
#include "util.h"

#define PK_SIZE 10 * sizeof(uint8_t)

// conn_new alloc a new conn_t struct
conn_t *conn_new(void)
{
	conn_t *conn = xzmalloc(sizeof(conn_t));

	conn->bufflen = PK_SIZE;
	conn->buff	= xzmalloc(conn->bufflen);
	conn->addr	= xzmalloc(sizeof(struct sockaddr_storage));

	return conn;
}

// conn_free frees the new ipv4/ipv6 connection
void conn_free(conn_t *conn)
{
	if (!conn)
		INFOEE("Can't free a connection pointer that is NULL");
	if ((!conn->addr) || (!conn->buff))
		INFOEE("Can't free a connection members that are NULL");

	xfree(conn->addr);
	xfree(conn->buff);
	xfree(conn);
}

// conn_addr_setup
// fill up the underlying addr of conn_t with the first valid dns lookup
void conn_addr_setup(conn_t *conn, conn_hints info)
{
	if (!conn)
		INFOEE("Can't use invalid connection, pointer is NULL");
	struct addrinfo *servinfo = NULL;
	struct addrinfo *p		  = NULL;
	int err					  = 0;
	int sk					  = 0;

	err	= getaddrinfo(info.host, info.proto, &info.hints, &servinfo);
	if (err)
		INFOEE("Can't get the ip addr from the dns lookup");

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sk = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (!sk)
			continue;
		else {
			conn->sock			  = sk;
			memcpy(conn->addr, p->ai_addr, p->ai_addrlen);
			break;
		}
	}

	freeaddrinfo(servinfo);
	// if p is NULL that means we looped off the end of the list
	if (p == NULL) {
		INFOEE("Can't find a valid ip4 address for the host you provided.");
	}
}
