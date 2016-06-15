# Project name
project( blackship C )

# set the verbose mode on
set(CMAKE_VERBOSE_MAKEFILE ON)

# SPECIFY THE MINIMUM VERSION OF CMAKE
cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
# Set all modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")
# include all external modules
include(def)
include(macros)
#TODO
include(CheckIncludeFile)
include(CheckLibraryExists)

CHECK_INCLUDE_FILE("argp.h" ARGP_LIBRARY CMAKE_REQUIRED_FLAGS)
CHECK_INCLUDE_FILE("cmocka.h" CMOCK_LIBRARY CMAKE_REQUIRED_FLAGS)
CHECK_LIBRARY_EXISTS("argp" "argp_parse" "/usr/include/argp.h" CMAKE_REQUIRED_FLAGS)

# set the new policy
cmake_policy(SET CMP0002 NEW)

# min version required
cmake_minimum_required(VERSION 2.6)

include_directories(lib)
include_directories(src)

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
				lib/util/mem.c)

add_executable(blackship ${TARGET_MAIN})

welcome()
