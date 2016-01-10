#ifndef SCTP_H
#define SCTP_H

#include "../../../../lib/sailfish.h"

void sctpstr_cli_echoall(FILE *, int, struct sockaddr *, socklen_t);
void sctpstr_cli(FILE *, int, struct sockaddr *, socklen_t);

#endif
