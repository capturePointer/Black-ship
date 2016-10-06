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

SET(ALL_FILES ../lib/info.c
			  ../lib/info.h
			  ../lib/util.h
			  ../lib/util.c
			  ../lib/err.h
			  ../lib/err.c
			  ../lib/pcg.c
			  ../lib/pcg.h
			  ../lib/conn.c
			  ../lib/conn.h
			  ../lib/err.c
			  ../lib/err.h
			  ../lib/mem.h
			  ../lib/mem.c
			  ../src/udp_flood.c
			  ../src/udp_flood.h
			  ../src/cmds.h
			  ../src/cmds.c
			  ../src/opts.c
			  ../src/opts.h)

# add all files and push them to exec
set (ERR_TEST_LIB err_test.c ${ALL_FILES})
set (UTIL_TEST_LIB util_test.c ${ALL_FILES})
set (BLACKSHIP_TEST_SRC blackship_test.c ${ALL_FILES})
set (CMDS_TEST_SRC cmds_test.c ${ALL_FILES})
set (MEM_TEST_LIB mem_test.c ${ALL_FILES})
set (CONN_TEST_LIB conn_test.c ${ALL_FILES})
set (OPTS_TEST_SRC opts_test.c ${ALL_FILES})

add_executable(err_test ${ERR_TEST_LIB})
add_executable(util_test ${UTIL_TEST_LIB})
add_executable(blackship_test ${BLACKSHIP_TEST_SRC})
add_executable(cmds_test ${CMDS_TEST_SRC})
add_executable(mem_test ${MEM_TEST_LIB})
add_executable(conn_test ${CONN_TEST_LIB})
add_executable(opts_test ${OPTS_TEST_SRC})

target_link_libraries(err_test cmocka)
target_link_libraries(util_test cmocka)
target_link_libraries(blackship_test cmocka)
target_link_libraries(cmds_test cmocka)
target_link_libraries(mem_test cmocka)
target_link_libraries(conn_test cmocka)
target_link_libraries(opts_test cmocka)


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

