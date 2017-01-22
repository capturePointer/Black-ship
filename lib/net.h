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
#ifndef CONN_H
#define CONN_H

#include <stdint.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

/**
 * tcp4_checksum
 *
 */
extern uint16_t tcp4_checksum(struct iphdr *ip, struct tcphdr *tcp);

/**
 * ip4_checksum
 */
extern uint16_t ip4_checksum(struct iphdr *header);

/**
 * addr_if 
 */
extern in_addr_t addr_if(const char *name, unsigned short family);
#endif /*CONN_H*/
