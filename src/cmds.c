#include <stdio.h>
#include <string.h>

#include "cmds.h"

// delcare here the list of attacks that the app will support
// informative const list , this list will change over time
// on the course of dev/mantaining this cmd app
static const char *attacks[] = {
	"udp - upd flood",
	"icmp - icmp flood",
	"igmp - igmp flood",
	"syn - tcp syn flood",
	"rst - tcp rst flood",
	"psh+ack - tcp PSH-ACK flood",
	"stress - sockstress type attack",
	"http - http flood",
	"dns - dns flood",
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
	while (*p != 0) {
		fprintf(stdout, "%s\n", *p);
		p++;
	}
}

bool valid_attack(const char *exploit)
{
	uint8_t i = 0;
	for (; i < N_ATTACKS; i++) {
		if (!strcmp(exploit, doses[i]))
			return true;
	}
	return false;
}

void run_cmd(arguments arg)
{
	printf("host : %s\n", arg.host);
	printf("port: %d\n", arg.port.n);
	printf("random: %d\n", (arg.port.random) ? 1 : 0);
	printf("low: %d , high: %d\n", arg.port.low, arg.port.high);
	printf("list_attacks: %d\n", (arg.list_attacks) ? 1 : 0);
}
