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
#ifndef INFO_H
#define INFO_H

#include <stdbool.h>

extern void info(const char *msg, const char *file, int line);
extern void infoee(const char *msg, const char *file, int line);
extern void wstatus(const char *msg);
extern void status(const char *msg);
extern void debug(const char *msg, const char *file, int line);
extern void debugf(const char *fmt, ...);
extern void active_debug(const bool f);

#define INFO(message) \
	info((message), __FILE__, __LINE__)

#define INFOEE(message) \
	infoee((message), __FILE__, __LINE__)

#define WSTATUS(message) \
	wstatus(message)

#define STATUS(message) \
	status(message)

#define DEBUGF(format, ...) \
	debugf(format, __VA_ARGS__)

#define DEBUG(msg)\
	debug((msg), __FILE__, __LINE__)

#define DEBUG_ON() \
	active_debug(true);

#define DEBUG_OFF() \
	active_debug(false);

#endif /*INFO_H*/
