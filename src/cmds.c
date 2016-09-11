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

#include <lib/util.h>
#include <lib/conn.h>
#include <lib/mem.h>
#include <lib/info.h>

#include "cmds.h"

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

#define N_ATTACKS 9
static const char *doses[] = {
	"udp", "icmp", "igmp",
	"syn", "rst", "psh+ack",
	"stress", "http", "dns",
	0,
};


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
// test if the exploit is valid and supported
// and return it as a flag
ATTACK_SW valid_attack(const char *exploit)
{
	uint8_t i = 0;
	for (; i < N_ATTACKS; i++) {
		if (!strcmp(exploit, doses[i]))
			switch(i) {
				case 0:
					return UDP_FLOOD;
				case 1:
					return ICMP_FLOOD;
				case 2:
					return IGMP_FLOOD;
				case 4:
					return  SYN_FLOOD;
				case 5:
					return RST_FLOOD;
				case 6:
					return PSH_ACK_FLOOD;
				case 7:
					return SOCKSTRESS;
				case 8:
					return HTTP_FLOOD;
				case 9:
					return DNS_FLOOD;
			}
	}
	return END_ATTACK;
}

// run_cmd is the entry point of the hole app , this will figure out
// what type of attack we want to start and launch it
void run_cmd(arguments arg)
{
	if(arg.list_attacks) {
		list_attacks();
		return;
	}
	
	// test if the host port and range port are set
	if ((strlen(arg.host) == 0) ||
		(!arg.port.n) || 
		(!arg.port.high)) {
			WSTATUS("Please set the host and port");
			return;
	}

	STATUS("Blackship start sailing..");
	// decide what attack we should launch
	switch(arg.attack) {
		case UDP_FLOOD:
			//udp_flood(arg);
			break;
		case ICMP_FLOOD:
			break;
		case IGMP_FLOOD:
			break;
		case SYN_FLOOD:
			break;
		case RST_FLOOD:
			break;
		case PSH_ACK_FLOOD:
			break;
		case SOCKSTRESS:
			break;
		case HTTP_FLOOD:
			break;
		case DNS_FLOOD:
			break;
		// if we reached this point that means something bad happened.
		case END_ATTACK:
			INFOEE("Can't run this attack, something is wrong");
	}

	/*DEBUG*/	
	printf("host : %s\n", arg.host);
	printf("port: %d\n", arg.port.n);
	printf("random: %d\n", (arg.port.random) ? 1 : 0);
	printf("low: %d , high: %d\n", arg.port.low, arg.port.high);
	printf("list_attacks: %d\n", (arg.list_attacks) ? 1 : 0);
}
