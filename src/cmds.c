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
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lib/conn.h>
#include <lib/info.h>
#include <lib/mem.h>
#include <lib/util.h>

#include "cmds.h"

// delcare here the list of attacks that the app will support
// informative const list , this list will change over time
// on the course of dev/mantaining this cmd app
static const char *attacks[] = {
	"stress	 - sockstress type attack",
	0,
};

static const char *doses[] = {
	"stress",
	0,
};

#define N_ATTACKS 1

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
					return SOCKSTRESS;
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

	treat_signal(SIGPIPE, SIG_IGN);
	// All things that is global we should handle here.
	// Every attack is different it requires different options
	// so we don't need to test them all here instead, we should
	// try to write independend code for every submodule to explicitly check for those.
	// we at least should check if the host is set or not at least
	if (!args.host) {
		WSTATUS("Please set the host with a valid ip");
		return;
	}

	if ((args.port.n == 0) && (args.port.low == 0) && (args.port.high == 0) && (!args.port.random)) {
		WSTATUS("Empty port number and port options, please set a valid one");
		return;
	}

	STATUS("Blackship start sailing..");

	conn_t *conn = conn_new();
	conn_buff_new(conn, args.packet.size);
	urandom_bytes(conn->buff, conn->bufflen); // fill up the buffer with random data

	// decide what attack we should launch
	// for every case stmt there will be an entry point function
	// with the name of <module-name>_attack
	switch (args.attack) {
	case SOCKSTRESS:
		break;
	// if we reached this point that
	// means something we the user passed invalid or unsupported attack
	case START_ATTACK:
	case END_ATTACK:
		WSTATUS("Please set a valid attack that the app supports");
	}

	conn_buff_free(conn);
	conn_free(conn);
}
