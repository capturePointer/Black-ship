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

#include "conn.h"
#include "mem.h"
#include "info.h"

#include <src/opts.h>

// conn_new alloc new ipv4, ipv6 connection
conn_t *conn_new(ip_t version)
{
	conn_t *conn = NULL;

	conn = xzmalloc(sizeof(conn_t));

	switch(version) {
		case IPV4:
			conn->c4 = xzmalloc(sizeof (conn4_t));
			conn->c4->addr = xzmalloc(sizeof(addr4_t));
			break;
		case IPV6:
			conn->c6 = xzmalloc(sizeof(conn6_t));
			conn->c6 = xzmalloc(sizeof(addr6_t));
			break;
	}
	
	return conn;
}

// conn_addr4_setup setup the underlying ipv4 addr struct of the conn_t 
void conn_addr4_setup(conn_t *conn, char *const host, const uint16_t port)
{
	if(!conn)
		INFOEE("[ERROR] Please provide a valid conn ptr");
	if(!conn->c4)
		INFOEE("[ERROR] Please provide a valid conn4 ptr");

	// TODO(hoenir) better use here the getaddrinfo logic for constructing addr4/addr6 conn
	conn->c4->addr->sin_port = port;
	conn->c4->addr->sin_family = AF_INET;

	if(!inet_pton(AF_INET, host, &conn->c4->addr->sin_addr))
		INFOEE("[ERROR] Could not parse the ip host address");
}

// conn_addr6_setup setup the underlying ipv6 addr struct of the conn_t 
void conn_addr6_setup(conn_t *conn, char *const host, const uint16_t port)
{
	if(!conn)
		INFOEE("[ERROR] Please provide a valid conn ptr");
	if(!conn->c4)
		INFOEE("[ERROR] Please provide a valid conn6 ptr");

	// TODO(hoenir) better use here the getaddrinfo logic for constructing addr4/addr6 conn
	conn->c6->addr->sin6_port = port;
	conn->c6->addr->sin6_family = AF_INET6;
	
	if(!inet_pton(AF_INET, host, &conn->c6->addr->sin6_addr))
		INFOEE("[ERROR] Could not parse the ip6 host address");

}
