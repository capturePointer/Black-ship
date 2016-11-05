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

// define list of colors
// this is just temrinal friendly
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"


// INFO_OPT
//
// string format options
typedef enum INFO_OPT {
	ERR_EX,		// print in a error format and exit the program
	WA,			// print in a warning format
	INF,		// print in a info format
	STAT,		// print in a status format
	DBG,        // print in a debug format
} INFO_OPT;

/**
 * inf
 *
 * @msg - string message format
 * @file - usually it's used in conjunction with __FILE__ macro
 * @line - usually it's used in conjunction with __LINE_ macro
 * @opt - one of the extra option on how you want the string to be formated
 *
 * writes to stdout the msg plus the file and line 
 * where the inf is called
 */
extern void inf(const char *msg,const char *file, int line, INFO_OPT opt);

#define INFO(message) \
	inf((message), __FILE__, __LINE__, INF)

#define INFOEE(message) \
	inf((message), __FILE__, __LINE__, ERR_EX)

#define WSTATUS(message) \
	inf((message), "" , 0, WA)

#define STATUS(message) \
	inf((message), "", 0, STAT)

#define DEBUG(message)	\
		inf(message, __FILE__, __LINE__, DBG);

#endif /*INFO_H*/
