#ifndef UDP_FLOOD_H
#define UDP_FLOOD_H

#include <lib/conn.h>

#include "src/opts.h"

// udp_flood_init entry point of the udp_flood module
// prepares all data types for launching the attack based on the
// connection and config arguments
extern void udp_flood_init(conn_t *conn, arguments args);

// checks if the current underlying implementation is set 
// this function should be call before calling udp_flood_init
extern void udp_flood_attack(void);

#endif
