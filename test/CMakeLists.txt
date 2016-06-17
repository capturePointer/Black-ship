# Project name
project(test C )

# SPECIFY THE MINIMUM VERSION OF CMAKE
cmake_minimum_required(VERSION 2.8)

# set the new policy
cmake_policy(SET CMP0002 NEW)

# set the verbose mode on
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER "/usr/bin/gcc")

# add all files and push them to exec
set (TARGET_MAIN ../lib/err/err.c 
				../lib/err/err.h
				../lib/util/info.c 
				../lib/util/info.h
				../lib/util/util.h
				../lib/util/mem.c
				err/err_test.c)

add_executable(err_test ${TARGET_MAIN})
target_link_libraries(err_test cmocka)

