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
#ifndef OPTS_H
#define OPTS_H

#include <argp.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct argp_state argp_state;
typedef struct argp_option argp_option;

#define DOC "This tool should be used just for educational \
purpose but I don't care(in the end) what you do \
with the code. I hope this will be a fine experience \
for me as a C developer and hope other guys in the \
future will contribute on this project to make it \
much cooler.So my main idea is to write a friendly \
command line Dos(Denial of service) tool."

extern const char *doc;
extern int parse_opt(int, char *, argp_state *);
extern const char *argp_program_version;
extern const char *argp_program_bug_address;
extern struct argp_option options[];
extern struct argp argp;

typedef enum ip_t {
	IPV4,
	IPV6,
} ip_t;

typedef struct port_t {
	// default standard port
	uint16_t n;
	// range values ports
	uint16_t low;
	uint16_t high;
	// flag for knowing that the port option
	// is set to random/dynamic ports
	bool random;

} port_t;
typedef enum other_t {
	NO_LIST,
	L_ATTACKS,					  // list attacks
} other_t;

// arguments retains all info from the parse_opt command
// when it finishes
typedef struct arguments {
	char *host;
	char *attack;
	ip_t host_type;
	port_t port;
	other_t list_attacks;
} arguments;

#endif
