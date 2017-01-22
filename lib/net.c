// Copyright [2016] [hoenir]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <string.h>

#include "info.h"
#include "mem.h"
#include "net.h"

in_addr_t addr_if(const char *name, unsigned short family)
{
	if (!name)
		INFOEE("Invalid interface name, pointer is NULL");

	struct ifaddrs *ifap;
	in_addr_t addr = 0;
	if (getifaddrs(&ifap) < 0)
		INFOEE("Cannot get interface list");

	for (struct ifaddrs *p = ifap; p != NULL; p = p->ifa_next) {
		if (strcmp(p->ifa_name, name) == 0)
			if (p->ifa_addr->sa_family == family) {
				addr = ((struct sockaddr_in *)(p->ifa_addr))->sin_addr.s_addr;
				break;
			}
	}

	freeifaddrs(ifap);
	return addr;
}

static uint16_t checksum(void *blk, size_t sz)
{
	if (!blk)
		INFOEE("Passing NULL ptr to checksum");

	uint16_t *raw  = blk;
	uint32_t sum   = 0;
	uint16_t carry = 0;

	while (sz > 1) {
		sum += *raw++;
		sz -= 2;
	}

	if (sz)
		sum += *(uint8_t *)raw;

	while ((carry = sum >> 16)) {
		sum += carry;
		sum = sum & 0xFFFF;
	}

	return (uint16_t)~sum;
}

uint16_t ip4_checksum(struct iphdr *header)
{
	if (!header) {
		DEBUG("Passing NULL ip header ptr to ip checksum");
		return 0;
	}

	return checksum(header, sizeof(*header));
}

typedef struct pseudohdr {
	uint32_t source;
	uint32_t dest;
	uint8_t zero;
	uint8_t protocol;
	uint16_t len;
} __attribute__((packed)) pseudohdr;

uint16_t tcp4_checksum(struct iphdr *ip, struct tcphdr *tcp)
{
	if (!ip)
		INFOEE("Cannot compute checksum, ip ptr is NULL");
	if(!tcp)
		INFOEE("Cannot compute checksum, tcp ptr is NULL");

	pseudohdr psu;
	memset(&psu, 0, sizeof(psu));
	psu.source   = ip->saddr;
	psu.dest	 = ip->daddr;
	psu.zero	 = 0;
	psu.protocol = ip->protocol;
	psu.len		 = htons(sizeof(*tcp));

	size_t len = sizeof(psu) + sizeof(*tcp);
	uint8_t buff[len];
	memset(&buff, 0, sizeof(buff));
	uint8_t *p = buff;
	memcpy(p, &psu, sizeof(psu));
	p += sizeof(psu);
	memcpy(p, tcp, sizeof(*tcp));
	return checksum(buff, len);
}
