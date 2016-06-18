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

#include <argz.h>
#include <stdlib.h>

#include "cmds.h"
#include "opts.h"

// define each key here
enum {
	LIST_ATTACKS = 10,
	PORT		 = 'p',
	ATTACK		 = 'a',
	RANGE_PORTS  = 'P',
	HOST		 = 'h',
	I4			 = 16,
	RANDOM		 = 'r',
};

// this array of options will be checked by the callback parse_opt
// in order to execute some special handlers on every interations
// parse_opt is called for every argv passed on the cmd line
struct argp_option options[] = {
	{ "attack", ATTACK, "type", 0,
	  "Select the type of attack. To know what types are supported"
	  "please check this options --list-attacks",
	  0 },
	{ "port", PORT, "n", 0,
	  "Select on a specific port to send all the packets",
	  0 },
	{ "range", RANGE_PORTS, "x-y", OPTION_ARG_OPTIONAL,
	  "Select a specic range of ports to send all the packets",
	  0 },
	{ "random", RANDOM, 0, OPTION_ARG_OPTIONAL,
	  "Just flood on random ports",
	  0 },
	{ "list-attacks", LIST_ATTACKS, 0, 0,
	  "List the dos floods supported",
	  0 },
	{ "host", HOST, "ip_addr", 0,
	  "Specify the ipv4 address.",
	  0 },
	{ "ip4", I4, "ipv4", 0,
	  "Ipv4 address.The reason this option exists it's because in the future the attacks will support ipv6 addr", 0 },
	{ NULL, 0, NULL, 0, NULL, 0 } /*end of the arr*/
};

// set other information options for the argp to display
// set the current version of the app
const char *argp_program_version = "0.1";
// set the email dest addr for bug reports
const char *argp_program_bug_address = "hoenirvili@gmail.com";

#define USAGE_DOC "HOST PORT TYPE_OF_ATTACK EXTRA-OPTIONS"

// here we add all the option declared above this struct
// this struct is the main configuration for the argp
// parsing library.
struct argp argp = {
	// assing all the options
	options,
	// assign the cb for parssing all the args
	parse_opt,
	// usage documentation
	USAGE_DOC,
	// add extra documenatation
	DOC,
	// leave other to default
	0, 0, 0,
};
//
// callback for argp_parse to call
// write in our body for every key handler function
int parse_opt(int key, char *arg, argp_state *state)
{
	(void)state;
	(void)arg;

	switch (key) {
	case ATTACK:
		break;
	case PORT:
		break;
	case RANGE_PORTS:
		break;
	case RANDOM:
		break;
	case LIST_ATTACKS:
		//TODO
		list_attacks();
		break;
	case HOST:
		break;
	case I4:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return EXIT_SUCCESS;
}
