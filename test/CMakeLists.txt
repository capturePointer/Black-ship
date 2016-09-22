# Project name
project(test C )

# SPECIFY THE MINIMUM VERSION OF CMAKE
cmake_minimum_required(VERSION 2.8)

# set the compile flags for every .c 
set(COMPILE_FLAGS "-std=gnu11 -I ../../ -g")
set(CMAKE_C_FLAGS  "${COMPILE_FLAGS}")


# set the new policy
cmake_policy(SET CMP0002 NEW)

# set the verbose mode on
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER "/usr/bin/gcc")

# add all files and push them to exec
set (ERR_TEST_LIB ../lib/err.c 
				../lib/err.h
				../lib/info.c 
				../lib/info.h
				../lib/util.h
				../lib/mem.c
				../lib/mem.h
				err_test.c)

set (UTIL_TEST_LIB ../lib/err.c 
				../lib/err.h
				../lib/info.c 
				../lib/info.h
				../lib/pcg.c
				../lib/pcg.h
				../lib/mem.c
				../lib/mem.h
				../lib/util.c
				../lib/util.h
				util_test.c)

set (BLACKSHIP_TEST_SRC ../src/cmds.h
					../src/cmds.c
					../src/opts.c
					../src/udp_flood.c
					../src/udp_flood.h
					../lib/conn.c
					../lib/conn.h
					../src/opts.h
					../lib/err.h
					../lib/err.c
					../lib/info.c
					../lib/info.h
					../lib/pcg.c
					../lib/pcg.h
					../lib/util.c
					../lib/mem.c
					../lib/mem.h
					blackship_test.c)
set (CMDS_TEST_SRC ../src/cmds.h
				../src/cmds.c
				../src/opts.c
				../src/opts.h
				../src/udp_flood.c
				../src/udp_flood.h
				../lib/err.h
				../lib/err.c
				../lib/info.c
				../lib/info.h
				../lib/pcg.c
				../lib/pcg.h
				../lib/util.c
				../lib/mem.c
				../lib/mem.h
				../lib/conn.h
				../lib/conn.c
				cmds_test.c)

set (MEM_TEST_LIB ../lib/mem.h
				../lib/mem.c
				../lib/info.h
				../lib/info.c
				../lib/err.h
				../lib/err.c
				  mem_test.c)

set (CONN_TEST_LIB ../lib/err.h
				../lib/err.c
				../lib/info.c
				../lib/info.h
				../lib/util.c
				../lib/pcg.c
				../lib/pcg.h
				../lib/mem.c
				../lib/mem.h
				../lib/conn.h
				../lib/conn.c
				conn_test.c)

add_executable(err_test ${ERR_TEST_LIB})
add_executable(util_test ${UTIL_TEST_LIB})
add_executable(blackship_test ${BLACKSHIP_TEST_SRC})
add_executable(cmds_test ${CMDS_TEST_SRC})
add_executable(mem_test ${MEM_TEST_LIB})
add_executable(conn_test ${CONN_TEST_LIB})
target_link_libraries(err_test cmocka)
target_link_libraries(util_test cmocka)
target_link_libraries(blackship_test cmocka)
target_link_libraries(cmds_test cmocka)
target_link_libraries(mem_test cmocka)
target_link_libraries(conn_test cmocka)


message(STATUS "[*] Verifying external libraries")

find_library(CMAKE_LIB
	NAMES cmocka cmoka.h
	HINTS "/usr/include /usr/lib /usr/local/lib /usr/local/include"
)
if(NOT ${CMOCKA_LIB})
	message(FATAL_ERROR "[!} PLEASE INSTALL MISSING LIBRARY")
else()
	message(STATUS "[+] Cmoka library - found")
endif()

