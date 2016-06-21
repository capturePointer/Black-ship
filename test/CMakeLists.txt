# Project name
project(test C )

# SPECIFY THE MINIMUM VERSION OF CMAKE
cmake_minimum_required(VERSION 2.8)

# set the compile flags for every .c 
set(COMPILE_FLAGS "-std=gnu11 -g")
SET(CMAKE_C_FLAGS  "${COMPILE_FLAGS}")

# set the new policy
cmake_policy(SET CMP0002 NEW)

# set the verbose mode on
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER "/usr/bin/gcc")

# add all files and push them to exec
set (ERR_TEST_LIB ../lib/err/err.c 
				../lib/err/err.h
				../lib/util/info.c 
				../lib/util/info.h
				../lib/util/util.h
				../lib/util/mem.c
				err/err_test.c)

set (UTIL_TEST_LIB ../lib/err/err.c 
				../lib/err/err.h
				../lib/util/info.c 
				../lib/util/info.h
				../lib/util/util.h
				../lib/util/mem.c
				../lib/util/util.c
				util/util_test.c)

add_executable(err_test ${ERR_TEST_LIB})
add_executable(util_test ${UTIL_TEST_LIB})

target_link_libraries(err_test cmocka)
target_link_libraries(util_test cmocka)

