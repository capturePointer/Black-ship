#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <lib/info.h>
#include <lib/util.h>

#include "sockstress.h"

typedef void (*sockstress_attack_fn)(void);

void sockstress_init(conn_t *conn, arguments args)
{
	//TODO
	(void)args;
	if (!conn)
		INFOEE("Invalid connection pointer passed");

}


extern void sockstress_attack(void)
{
}
