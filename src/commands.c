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

#include <lib/info.h>
#include <lib/util.h>
#include <lib/mem.h>

#include "commands.h"
#include "sockstress.h"

static const char *attacks[] = {
	"stress	 - sockstress type attack",
};

static const char *doses[] = {
	"stress",
};


void list_attacks(void)
{
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

static void sigint_handler (int signo)
{
	fprintf(stdout, "\n");
	if (signo != SIGINT)
		INFOEE("Cannot treat any signal different from SIGINT");

	STATUS("Closing connection and freeing up the memory");	
	exit(EXIT_SUCCESS);
}

void run_cmd(arguments args)
{
	if (args.list_attacks) {
		list_attacks();
		return;
	}

	if (treat_signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		INFOEE("Cannot set signal handler for SIGPIPE");
	if (treat_signal(SIGINT, sigint_handler) == SIG_ERR)
		INFOEE("Cannot not set the signal handler for SIGINT");

	if (!args.host) {
		WSTATUS("Please set the host with a valid ip");
		return;
	}

	if (args.port.n == 0) {
		WSTATUS("Empty port number and port options, please set a valid one");
		return;
	}

	STATUS("Blackship start sailing..");

	impl_t *impl = NULL;

	/**
	 * based on the args.attack enum we should
	 * pick what kind of implementaton we are interesed on
	 * every attack returns a ptr to impl_t *
	 * every attack implementation has 3 esential methods
	 * one for init, one for the attack and one for freeing up resources when
	 * the attack is stopped.
	 */
	switch (args.attack) {
		case SOCKSTRESS: {
			STATUS("Chosen to launch Sockstress attack");
			impl = sockstress_new();
			break;
		}

		case START_ATTACK:{}
		case END_ATTACK: {
			WSTATUS("Please set a valid attack that the app supports");
			return;
		}
	} /*switch*/
	
	if (!impl)
		INFOEE("No attack has been initilized properly");

	impl->init(args);
	impl->attack();
	impl->clean();


	xfree(impl);
}
