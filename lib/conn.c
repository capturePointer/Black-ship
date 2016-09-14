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

// conn_new alloc new ipv4, ipv6 connection
// if a invalid version is passed the return will be NULL
conn_t *conn_new(ip_t version)
{
	conn_t *conn = xzmalloc(sizeof(conn_t));
	switch (version) {
	case IPV4:
		conn->c4	   = xzmalloc(sizeof(conn4_t));
		conn->c4->addr = xzmalloc(sizeof(addr4_t));
		break;
	case IPV6:
		conn->c6	   = xzmalloc(sizeof(conn6_t));
		conn->c6->addr = xzmalloc(sizeof(addr6_t));
		break;
	}

	return conn;
}

// conn_free frees the new ipv4, ipv6 connection
// if you pass a NULL on conn this will throw an error and 
// it will exit.
void conn_free(conn_t *conn, ip_t version)
{
	if (!conn)
		INFOEE("Can't free a connection pointer that is NULL");

	switch (version) {
	case IPV4:
		if ((!conn->c4) || (!conn->c4->addr)) 
			INFOEE("Can't free members of the connection that is NULL");
		xfree(conn->c4->addr);
		xfree(conn->c4);
		break;
	case IPV6:
		if((!conn->c6) || (!conn->c6->addr))
			INFOEE("Can't free members of the connection that is NULL");
		xfree(conn->c6->addr);
		xfree(conn->c6);
		break;
	}

	xfree(conn);
}

// conn_addr4_setup
// setup the underlying ipv4 addr struct of the conn_t
// will decorate it with the first valid socket and init with info about the addr type
// that the socket will use in order to connect on that target.
void conn_addr4_setup(conn_t *conn, conn_hints info)
{
	if ((!conn) || (!conn->c4) || (!conn->c4->addr))
		INFOEE("Please provide a valid conn ptr");

	struct addrinfo *servinfo = NULL;
	struct addrinfo *p		  = NULL;
	int err					  = 0;
	int sk					  = 0;

	err = getaddrinfo(info.host, info.proto, &info.hints, &servinfo);
	if (err) {
		INFOEE("Could'n not get the ip addr and init the addr info");
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sk = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (!sk) {
			continue;
		} else {
			conn->c4->sock = sk;
			memcpy(conn->c4->addr, p->ai_addr, sizeof(addr4_t));
			conn->c4->addr->sin_port = port_conv(info.proto);
			// if proto_conv can't convert the port number
			// assign it a default value.
			if (err_this(ERRCONVPORT)) {
				STATUS("Can't convert proto specified into a valid port number");
				STATUS("Default port number assigned is 0.");
				conn->c4->addr->sin_port = 0;
			}
			break;
		}
	}

	freeaddrinfo(servinfo);

	// if p is NULL that means we looped off the end of the list
	if (p == NULL) {
		INFOEE("Can't find a valid ip4 address for the host you provided.");
	}
}

// conn_addr6_setup
// setup the underlying ipv6 addr struct of the conn_t
// will decorate it with the first valid socket and init with info about the addr type
// that the socket will use in order to connect on that target.

void conn_addr6_setup(conn_t *conn, conn_hints info)
{
	if ((!conn) || (!conn->c6) || (!conn->c6->addr))
		INFOEE("Please provide a valid conn ptr");

	struct addrinfo *servinfo = NULL;
	struct addrinfo *p		  = NULL;
	int err					  = 0;
	int sk					  = 0;

	err = getaddrinfo(info.host, info.proto, &info.hints, &servinfo);
	if (err) {
		INFOEE("Could'n not get the ip addr and init the addr info");
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sk = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (!sk) {
			continue;
		} else {
			conn->c6->sock = sk;
			memcpy(conn->c6->addr, p->ai_addr, sizeof(addr6_t));
			conn->c6->addr->sin6_port = port_conv(info.proto);
			if (err_this(ERRCONVPORT)) {
				STATUS("Can't convert proto specified into a valid port number");
				STATUS("Default port number assigned is 0.");
				conn->c6->addr->sin6_port = 0;
			}
			break;
		}
	}

	freeaddrinfo(servinfo);

	// if p is NULL that means we looped off the end of the list
	if (p == NULL) {
		INFOEE("Can't find a valid ip6 address for the host you provided.");
	}
}
