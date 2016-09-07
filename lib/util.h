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
#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdbool.h>

extern uint16_t port_conv(const char *arg);
extern void port_conv_range(char *arg, uint16_t *low, uint16_t *high);
extern bool filter_number(const char *arg);
extern bool valid_ip(const char *ip);
extern uint16_t port_random(void);
extern uint16_t pcg_random_xorshit(void);
#endif /*UTIL_H*/
