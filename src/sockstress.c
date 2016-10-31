#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <lib/info.h>
#include <lib/util.h>

typedef void (*sockstress_attack_fn)(void);

void sockstress_init(conn_t *conn, arguments args)
{
	if (!conn)
		INFOEE("Invalid connection pointer passed");

	struct sockaddr_in in;
	struct sockaddr_in in6;
	
	switch(args.host_type) {
		case IPV4:
			in.sin_family = AF_INET;
			inet_pton(AF_INET, args.host, &in.sin_addr);
			memcpy(conn->addr, &in, sizeof(in));
			break;
		case IPV6:
			in.sin_family = AF_INET6;
			inet_pton(AF_INET, args.host, &in.sin_addr);
			memcpy(conn->addr, &in6, sizeof(in6));
			break;
		default:
			INFOEE("Neighter ipv4 or ipv6 is set up");
	}

	conn->sock = socket(IP_SECP(args.host_type), SOCK_STREAM, IPPROTO_TCP);
	if (conn-sock == -1) {
		INOFEE("Can't create tcp socket");
	}
}

