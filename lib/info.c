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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"


// info prints out to stdout the msg, file and line number
inline void info(const char *msg, const char *file, int line)
{
	fprintf(stdout, "[ * ] %s %s %d\n", msg, file, line);
}

// info prints out to stdout the message, file and the line number
// after the message is writed to stdout, it exits the program
inline void infoee(const char *msg, const char *file, int line)
{
	fprintf(stderr, "[ ! ] %s %s %d\n", msg, file, line);
	exit(EXIT_FAILURE);
}

inline void wstatus (const char *msg)
{
	fprintf(stdout, "[ ! ] %s\n", msg);
}

inline void status (const char *msg)
{
	fprintf(stdout,"[ * ] %s\n" , msg);
}
