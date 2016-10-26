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

#include <stdlib.h>
#include <argz.h>

#include <lib/info.h>
#include <lib/util.h>

#include "cmds.h"
#include "opts.h"

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
	{ "packets",PACKETS, "n", 0,
		"Number of packets that will be send to the host",
		0},
	{ "packet-size", PACKETS_SZ, "size", 0,
		"Specify the size of the packet. Max size 1<<16",
		0},
	{ "i4", I4, 0, 0,
	  "Ipv4 address.", 0 },
	{ "i6", I6, 0, 0,
	   "Ipv6 address.", 0},
	{ NULL, 0, NULL, 0, NULL, 0 } /*end of the arr*/
};

// set other information options for the argp to display
// set the current version of the app
const char *argp_program_version = "0.1";
// set the email dest addr for bug reports
const char *argp_program_bug_address = "hoenirvili@gmail.com";


#define USAGE_DOC "HOST PORT TYPE_OF_ATTACK EXTRA-OPTIONS"

#define DOC "This tool should be used just for educational \
	purpose but I don't care(in the end) what you do \
with the code. I hope this will be a fine experience \
for me as a C developer and hope other guys in the \
future will contribute on this project to make it \
much cooler.So my main idea is to write a friendly \
command line Dos(Denial of service) tool."

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
error_t parse_opt(int key, char *arg, argp_state *state)
{
	arguments *args   = state->input;
	state->err_stream = stderr;
	state->out_stream = stdout;
	
	switch (key) {

	case ATTACK: {
		// test if the attack is valid
		args->attack = valid_attack(arg);
		if (args->attack == END_ATTACK) {
			return ARGP_KEY_ERROR;
		}
		break;
	}

	case PORT: {
		int16_t p = port_conv(arg);
		if (!p) 
			return ARGP_KEY_ERROR;
		args->port.n = (uint16_t)p;
		break;
	}

	case RANGE_PORTS: {
		int16_t err = port_conv_range(arg, &args->port.low, &args->port.high);
		if(!err)
			return ARGP_KEY_ERROR;
		break;
	}

	case RANDOM: {
		args->port.random = true;
		port_seeds(); // start seeding immediately
		break;
	}

	case PACKETS: {
		int64_t packet = strconv(arg, 10);
		if (!packet)
			return ARGP_KEY_ERROR;
		args->packet.n  = (uint64_t)packet;
		break;
	}

	case PACKETS_SZ: {
		int64_t sz = strconv(arg, 10);
		if(!sz) 
			return ARGP_KEY_ERROR;
		args->packet.size = (uint16_t)sz;
		break;
	}

	case LIST_ATTACKS: {
		args->list_attacks = L_ATTACKS;
		break;
	}

	case HOST: {
		if (!valid_ip(arg))
			return ARGP_KEY_ERROR;
		args->host = arg;
		break;

	}

	case I4: {
		args->host_type = IPV4;
		break;
	}

	case I6: {
		args->host_type = IPV6;
		break;
	}

	default: {
		// not a valid option
		return ARGP_ERR_UNKNOWN;
	}
	} /*switch*/

	return 0;
}
