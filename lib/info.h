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

extern void info(const char *msg, const char *file, int line);
extern void infoee(const char *msg, const char *file, int line);
extern void wstatus(const char *msg);
extern void status(const char *msg);

#define INFO(message) \
	info((message), __FILE__, __LINE__)

#define INFOEE(message) \
	infoee((message), __FILE__, __LINE__)

#define WSTATUS(message) \
	wstatus(message)

#define STATUS(message) \
	status(message)

#endif /*INFO_H*/
