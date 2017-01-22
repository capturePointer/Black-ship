#ifndef SOCKSTRESS_H
#define SOCKSTRESS_H

#include "commands.h"

/**
 * sockstress_new
 *
 * function returns a concreate default implementation of the
 * sockstress attack
 */
impl_t *sockstress_new(void);

#endif /*SOCKSTRESS_H*/
