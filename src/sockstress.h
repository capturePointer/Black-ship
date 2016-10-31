#ifndef SOCKSTRESS_H
#define SOCKSTRESS_H

#include <lib/net.h>

#include "opts.h"

extern void sockstress_init(conn_t *conn, arguments args);

extern void sockstress_attack(void);
#endif /*SOCKSTRESS_H*/
