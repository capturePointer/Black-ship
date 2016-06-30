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

#include "opts.h"
#include "../lib/util/util.h"
#include "../lib/err/err.h"
#include "cmds.h"
#include <argz.h>
#include <stdlib.h>

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

// callback for argp_parse to call
// write in our body for every key handler function
int parse_opt(int key, char *arg, argp_state *state)
{
	arguments *args = state->input;
	switch (key) {
	case ATTACK:
		// select the type of attack
		args->attack = arg;
		break;
	case PORT:
		// select the single port and convert the string value
		// into uint16_t
		args->port.n = port_conv(arg);
		break;
	case RANGE_PORTS:
		// convert range port values into uint16_t
		port_conv_range(arg, &args->port.low, &args->port.high);
		// if there is error retun it to the parser
		if(err_this(ERRCONVPORT))
			return ARGP_KEY_ERROR;
		// continue
		break;
	case RANDOM:
		// assign random port option this will ignore all range port values 
		// and single port, it will send traffic to random ports
		args->port.random = true;
		break;
	case LIST_ATTACKS:
		// print out the list of attacks
		args->list_attacks = L_ATTACKS;
		break;
	case HOST:
		// select the target we will send all the traffic
		args->host = arg;
		break;
	case I4:
		// use ip version 4
		args->host_type = IPV4;
		break;
	default:
		// not a valid option
		return ARGP_ERR_UNKNOWN;
	}
	
	return EXIT_SUCCESS;
}
