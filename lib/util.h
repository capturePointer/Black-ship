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
#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// singfn more convinient way to write signal handlers
typedef void (*sigfn)(int);

extern bool filter_number(const char *);
extern uint16_t port_conv(const char *);
extern void port_conv_range(char *, uint16_t *, uint16_t *);
extern char *xsprintf(const char *, ...);
extern bool valid_ip(const char *);
extern bool urandom_bytes(void *dest, size_t);
extern void port_seeds(void);
extern uint16_t port_random(void);
extern uint64_t strconv(const char *, uint8_t);
extern sigfn treat_signal(int, sigfn fn);
#endif /*UTIL_H*/
