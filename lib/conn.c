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
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>

#include "conn.h"
#include "err.h"
#include "info.h"
#include "mem.h"
#include "util.h"

// conn_new alloc new ipv4, ipv6 connection
// if a invalid version is passed the return will be NULL
conn_t *conn_new(ip_t version)
{
	conn_t *conn = NULL;

	switch (version) {
	case IPV4:
		conn = xzmalloc(sizeof(conn_t));
		conn->c4	   = xzmalloc(sizeof(conn4_t));
		conn->c4->addr = xzmalloc(sizeof(addr4_t));
		break;
	case IPV6:
		conn = xzmalloc(sizeof(conn_t));
		conn->c6 = xzmalloc(sizeof(conn6_t));
		conn->c6->addr = xzmalloc(sizeof(addr6_t));
		break;
	}

	return conn;
}

// conn_addr4_setup setup the underlying ipv4 addr struct of the conn_t
void conn_addr4_setup(conn_t *conn, const char *host, const char *proto)
{
	if (!conn)
		INFOEE("Please provide a valid conn ptr");
	if (!conn->c4)
		INFOEE("Please provide a valid conn4 ptr");

	struct addrinfo hints, *servinfo, *p;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;			// auto
	int err, sk;

	err = getaddrinfo(host, proto, &hints, &servinfo);
	if (!err) {
		INFOEE("Could'n not get the ip addr and init the addr info");
	}

	for (p = servinfo; p != NULL; p++) {
		sk = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (!sk) {
			continue;
		} else {
			conn->c4->addr->sin_port = port_conv(proto);
			if (err_this(ERRCONVPORT)) {
				STATUS("Can't convert proto specified into a valid port number");
				STATUS("Default port number assigned is 0.");
				conn->c4->addr->sin_port = 0;
			}
			conn->c4->sock			   = sk;
			conn->c4->addr->sin_family = (sa_family_t)p->ai_family;
			conn->c4->addr			   = (addr4_t *)&p->ai_addr; // convert into addr4_t from sockaddr*
		}
		break;
	}
	
	// if p is NULL that means we looped off the end of the list
	if (p == NULL) {
		INFOEE("Can't find a valid ip4 address for the host you provided.");
	}

	freeaddrinfo(servinfo);
}

// conn_addr6_setup setup the underlying ipv6 addr struct of the conn_t
void conn_addr6_setup(conn_t *conn, const char *host, const char *proto)
{
	if (!conn)
		INFOEE("Please provide a valid conn ptr");
	if (!conn->c4)
		INFOEE("Please provide a valid conn6 ptr");

	struct addrinfo hints, *servinfo, *p;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;			// auto
	int err, sk;

	err = getaddrinfo(host, proto, &hints, &servinfo);
	if (!err) {
		INFOEE("Could'n not get the ip addr and init the addr info");
	}

	for (p = servinfo; p != NULL; p++) {
		sk = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (!sk) {
			continue;
		} else {
			conn->c6->addr->sin6_port = port_conv(proto);
			if (err_this(ERRCONVPORT)) {
				STATUS("Can't convert proto specified into a valid port number");
				STATUS("Default port number assigned is 0.");
				conn->c6->addr->sin6_port = 0;
			}
			conn->c6->sock			    = sk;
			conn->c6->addr->sin6_family = (sa_family_t)p->ai_family;
			conn->c6->addr			    = (addr6_t *)&p->ai_addr; // convert into addr6_t from sockaddr*
		}
		break;
	}
	
	// if p is NULL that means we looped off the end of the list
	if (p == NULL) {
		INFOEE("Can't find a valid ip6 address for the host you provided.");
	}

	freeaddrinfo(servinfo);
}
