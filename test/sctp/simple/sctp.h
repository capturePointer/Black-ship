#ifndef SCTP_H
#define SCTP_H
#include "../../../lib/sailfish.h"

int main_socket, client_socket;
int wr_sz, flags;
int i;
ssize_t rd_sz;

char buffer[MAXLINE];
socklen_t len;
int reuse;

// define all our structs
struct sctp_initmsg initmsg;
struct sctp_event_subscribe event;
struct sctp_sndrcvinfo sndrcvinfo;

#endif
