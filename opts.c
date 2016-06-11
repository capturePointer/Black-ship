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

// thic cmd tool uses argp as it's parser mechanism for the
// command line arguments

#include <argp.h>
#include <stdlib.h>

// define each key here
enum {
	LIST_FLOODS = 0,
	PORT		= 'p',
	FLOOD		= 'f',
	RANGE_PORTS = 'P',
	HOST		= 'h',
	IP6			= 1,
};

#include "opts.h"

// this array of options will be checked by the callback parse_opt
// in order to execute some special handlers on every interations
// parse_opt is called for every argv passed on the cmd line
struct argp_option options[] = {
	{ "flood", FLOOD, "type", 0,
	  "Select the type of flood. To know what types are supported"
	  "please check this options --list-flood",
	  0 },
	{ "port", PORT, "n", OPTION_ARG_OPTIONAL,
	  "Select on a specific port to send all the packets",
	  0 },
	{ "range-ports", RANGE_PORTS, "p-p", OPTION_ARG_OPTIONAL,
	  "Select a specic range of ports to send all the packets",
	  0 },
	{ "random", 'r', 0, OPTION_ARG_OPTIONAL,
	  "Just flood on random ports",
	  0 },
	{ "list-floods", LIST_FLOODS, 0, 0,
	  "List the dos floods supported",
	  0 },
	{ "host", HOST, "ip", 0,
	  "Specify the traget hostname or ipv4 addr. if you want use ipv6 please specify with -ip6 option",
	  0 },
	{ "ip6", IP6, "ipv6", OPTION_ARG_OPTIONAL,
	  "Ipv6 address", 0 },

	{ NULL, 0, NULL, 0, NULL, 0 } /*end of the arr*/
};

// delcare here the list of floods that the app will support
// informative const list , this list will change over time
// on the course of dev/mantaining this cmd app
const char *list_floods = "udp	- upd flood\n"
						  "syn	- tcp syn flood";
//
// set other information options for the argp to display
// set the current version of the app
const char *argp_program_version = "0.1";
// set the email dest addr for bug reports
const char *argp_program_bug_address = "hoenirvili@gmail.com";


#define USAGE_DOC "HOST TYPE_OF_ATTACK"

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
	switch (key) {
	case LIST_FLOODS:
		fprintf(stdout, "%s\n", list_floods);
		break;
	}

	return EXIT_SUCCESS;
}
