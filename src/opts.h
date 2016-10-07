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
#include <stdbool.h>
#include <stdint.h>

typedef struct argp_state argp_state;
typedef struct argp_option argp_option;

extern const char *doc;
extern error_t parse_opt(int, char *, argp_state *);
extern const char *argp_program_version;
extern const char *argp_program_bug_address;
extern struct argp_option options[];
extern struct argp argp;

// define each key here
enum {
	LIST_ATTACKS = 10,
	PORT		 = 'p',
	ATTACK		 = 'a',
	RANGE_PORTS  = 'P',
	HOST		 = 'h',
	I4			 = 14,
	I6			 = 16,
	RANDOM		 = 'r',
	DEBUG		 = 'd',
	PACKETS		 = 17,
};

// list of attacks that the cmd line will support
// we are using two sentinel values START_ATTACK and
// END_ATTACK, they are there looping and conditional
// testing purposes.
enum ATTACK_SW {
	START_ATTACK = 0, 
	UDP_FLOOD,
	ICMP_FLOOD,
	IGMP_FLOOD,
	SYN_FLOOD,
	RST_FLOOD,
	PSH_ACK_FLOOD,
	SOCKSTRESS,
	HTTP_FLOOD,
	DNS_FLOOD,
	END_ATTACK,	
} ATTACK_LIST;
typedef enum ATTACK_SW ATTACK_SW;

// port_t type that will hold all flags and port numbers
typedef struct port_t {
	uint16_t n;	// default standard port
	// range values ports
	uint16_t low;
	uint16_t high;
	// flag for knowing that the port option
	// is set to random/dynamic ports
	bool random;
} port_t;

// this type will be used for flag detection
// in order to know if we are using IPV4 or IPV6 conn
typedef enum ip_t {
	IPV4,
	IPV6,
} ip_t;

// other_t type that stores extra
// options for the app
typedef enum other_t {
	NO_LIST,
	L_ATTACKS,	// list attacks
} other_t;

// arguments retains all info from the parse_opt command
// when it finishes
typedef struct arguments {
	const char *host;
	ATTACK_SW attack;	
	ip_t host_type;
	port_t port;
	uint64_t packets;
	other_t list_attacks;
} arguments;

#endif
