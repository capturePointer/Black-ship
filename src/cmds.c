#include <stdio.h>
#include "cmds.h"

// delcare here the list of floods that the app will support
// informative const list , this list will change over time
// on the course of dev/mantaining this cmd app
const char *floods[] = { 
	"udp - upd flood",
	"syn - tcp syn flood",
	0,
};


void list_floods()
{
	const char **p = floods;
	while (*p != 0) {
		fprintf(stdout, "%s\n", *p);
		p++;
	}
}


