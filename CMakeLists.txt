# Project name
project( blackship C )

# SPECIFY THE MINIMUM VERSION OF CMAKE
cmake_minimum_required(VERSION 2.8)

# set the new policy
cmake_policy(SET CMP0002 NEW)

set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "1")
set(APPLICATION_VERSION_PATCH "0")

set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

# set the verbose mode on
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER "/usr/bin/gcc")

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set (CMAKE_MODULE_PATH 
	${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake"
)

include(Define)
include(Functions)

check_libs()

# add all files and push them to exec
set (TARGET_MAIN src/blackship.c 
				src/blackship.h
				src/cmds.h
				src/cmds.c 
				src/opts.c 
				src/opts.h
				lib/err/err.c 
				lib/err/err.h
				lib/util/info.c 
				lib/util/info.h
				lib/util/util.h
				lib/util/mem.c
				lib/util/util.c)
add_executable(blackship ${TARGET_MAIN})

