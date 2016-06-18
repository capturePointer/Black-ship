#include <stdio.h>
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

void list_attacks(void)
{
	const char **p = attacks;
	while (*p != 0) {
		fprintf(stdout, "%s\n", *p);
		p++;
	}
}
