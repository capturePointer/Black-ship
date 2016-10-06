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
#include <stdlib.h>
#include <signal.h>

#include <lib/conn.h>
#include <lib/info.h>
#include <lib/mem.h>
#include <lib/util.h>
#include <lib/err.h>

#include "cmds.h"
#include "udp_flood.h"

// delcare here the list of attacks that the app will support
// informative const list , this list will change over time
// on the course of dev/mantaining this cmd app
static const char *attacks[] = {
	"udp	 - upd flood",
	"icmp	 - icmp flood",
	"igmp	 - igmp flood",
	"syn	 - tcp syn flood",
	"rst	 - tcp rst flood",
	"psh+ack  - tcp PSH-ACK flood",
	"stress	 - sockstress type attack",
	"http	 - http flood",
	"dns	 - dns flood",
	0,
};

static const char *doses[] = {
	"udp", "icmp", "igmp",
	"syn", "rst", "psh+ack",
	"stress", "http", "dns",
	0,
};

#define N_ATTACKS 9

void list_attacks(void)
{
	const char **p = attacks;
	fprintf(stdout, "\n");
	STATUS("This pirate ship suports this attacks");
	while (*p != 0) {
		fprintf(stdout, "%s\n", *p);
		p++;
	}
	fprintf(stdout, "\n");
}

// valid_attack
// return the flag if the exploit is valid and supported
ATTACK_SW valid_attack(const char *exploit)
{
	uint8_t i = 0;
	for (; i < N_ATTACKS; i++) {
		if (!strcmp(exploit, doses[i])) {
			switch (i) {
			case 0:
				return UDP_FLOOD;
			case 1:
				return ICMP_FLOOD;
			case 2:
				return IGMP_FLOOD;
			case 3:
				return SYN_FLOOD;
			case 4:
				return RST_FLOOD;
			case 5:
				return PSH_ACK_FLOOD;
			case 6:
				return SOCKSTRESS;
			case 7:
				return HTTP_FLOOD;
			case 8:
				return DNS_FLOOD;
			}
		}
	}
	return END_ATTACK;
}

// run_cmd is the entry point of the hole app , this will figure out
// what type of attack we want to launch.
void run_cmd(arguments args)
{
	// if the list_attacks are set just echo and return
	if (args.list_attacks) {
		list_attacks();
		return;
	}
	
	// if write failures occures we want to handle them where the error
	// occurs rather than in a sigpipe handler.

	if (treat_signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		INFOEE("Failed to ingore the signal pipe");

	// All things that is global we should handle here.
	// Every attack is different it requires different options 
	// so we don't need to test them all here instead, we should 
	// try to write independend code for every submodule to explicitly check for those.
	// we at least should check if the host is set or not at least
	if (!args.host) {
		WSTATUS("Please set the host with a valid ip");
		return;
	}
	
	if ((args.port.n == 0) && (args.port.low == 0) && 
		(args.port.high == 0) && (!args.port.random)) {
		WSTATUS("Empty port number and port options, please set a valid one");
		return;
	}

	STATUS("Blackship start sailing..");

	conn_t *conn = conn_new();

	// decide what attack we should launch
	// for every case stmt there will be an entry point function
	// with the name of <module-name>_attack
	switch (args.attack) {
	case UDP_FLOOD:
		DEBUG("UDP_FLOOD attack is activated");
		udp_flood_init(conn, args);
		udp_flood_attack();
		break;
	case ICMP_FLOOD:
		DEBUG("ICMP_FLOOD attack is activated");
		break;
	case IGMP_FLOOD:
		DEBUG("IGCMP_FLOOD attack is activated");
		break;
	case SYN_FLOOD:
		DEBUG("SYN_FLOOD attack is activated");
		break;
	case RST_FLOOD:
		DEBUG("RST_FLOOD attack is activated");
		break;
	case PSH_ACK_FLOOD:
		DEBUG("PSH_ACK_FLOOD attack is activated");
		break;
	case SOCKSTRESS:
		DEBUG("SOCKSTRESS attack is activated");
		break;
	case HTTP_FLOOD:
		DEBUG("HTTP_FLOOD attack is activated");
		break;
	case DNS_FLOOD:
		DEBUG("DNS_FLOOD attack is activated");
		break;
	// if we reached this point that 
	// means something we the user passed invalid or unsupported attack
	case START_ATTACK:
	case END_ATTACK:
		WSTATUS("Please set a valid attack that the app supports");	
	}

	conn_free(conn);
}
