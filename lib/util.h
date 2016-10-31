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

/*
 * filter_number 
 *
 * @arg - string null terminated
 * 
 * filters all character and test if we have only digits 
 *
 * Return:
 *  if we have only digits it returns true
 *  if we have letters or other simbols it returns false
 */
extern bool filter_number(const char *arg);


/*
 * port_conv 
 *
 * @arg - port number in char* format
 *
 * convert from a char* representation to port number
 *
 * ports are defined as uint16_t because in ports are defined
 * are 16 byte wide and non negative.
 * max value for a port should be UINT16_MAX
 *
 * Return:
 *  if the conversion fails it returns -1
 *
 */
extern int16_t port_conv(const char *arg);

/*
 * port_conv_range
 *
 * @arg - string of range ports, the numbers must be splited with "-"
 *	example: 1234-5000
 * @low - the value low port of the inverval will be saved
 * @hig - the value where the high port of the interval will be saved
 *
 * Converts the string interval into port numbers. If the ports are in descending order
 * the @low and @high will still have the left and the right most interval value.
 *
 * Return:
 *  It returns -1 if the conversion fails otherwise it return 0;
 *  
 */
extern int8_t port_conv_range(char *arg, uint16_t *low, uint16_t *high);

/*
 * valid_ip 
 *
 * @ip - ptr to a string ip
 *
 * test if it's a valid ip(ipv4,ipv6) and return true
 * if it's not a valid ip return false
 *
 * Return:
 *	if Successful it returns true otherwise it returns false.
 */
extern bool valid_ip(const char *ip);

/*
 * urandom_bytes
 *
 * @dest - ptr to destination where we should start to fill up the random bytes
 * @size - the size of chunk that the @dest has.
 *
 * Use /dev/urandom to get some entropy bytes for seeding purposes.
 * The maximum size that can be the @dest should not be larger than 65535
 *
 * Return:
 *  if reading /dev/urandom fails(which ought to never happen), it returns
 *  false, otherwise it returns true.
 *
 */
extern bool urandom_bytes(void *dest, size_t size);

/**
 * port_seeds
 *
 * @dest - the destination where the bytes will be written
 * @size  - how big is the @dest
 *
 * fill up @dest with random bytes until we reached @size
 *
 * Return:
 *  the function will panic and exit the program if we can't read form the source of entropy
 * 
 * Note:
 *  the soruce of entropy that the function is using is /udev/random
 *  be sure if the this device dosen't exist in your platfom don't use this func.
 */
extern void port_seeds(void);

/**
 * port_random
 *
 * generates a valid port number from 1 to UINT16_MAX
 */
extern uint16_t port_random(void);

/* 
 * strconv
 *
 * @n	 - the string
 * @base - the base number
 *
 * convert the string n to a specific format byte
 *
 * Side note: very important that n ends with '\0' or this will have very bad side effects
 *
 * Return:
 *	if the conversion fails due to invalid @base or invalid @n string it will
 *	return 0 and ERRCONV into the err circular list.
 *	if the conversion succeeds it will return a valid number
 */

extern int64_t strconv(const char *n, uint8_t base);

/*
 * treat_signal
 *
 * @signo - specifies the signal values define in <signal.h>
 * @fn - pointer to a signal-catching function or one of the macros
 * SIG_IGN or SIG_DFL.
 *
 * friendly wrapper around sigaction
 * function allows the calling process to specify the action 
 * to be associated with a specific signal.
 *
 * Return:
 *	if any error occured it returns SIG_ERR or it will return the @fn
 */
extern sigfn treat_signal(int, sigfn fn);

#endif /*UTIL_H*/
