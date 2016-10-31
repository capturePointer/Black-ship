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
			  ../lib/pcg.c
			  ../lib/pcg.h
			  ../lib/net.c
			  ../lib/net.h
			  ../lib/mem.h
			  ../lib/mem.c
			  ../src/cmds.h
			  ../src/cmds.c
			  ../src/sockstress.c
			  ../src/sockstress.h
			  ../src/opts.c
			  ../src/opts.h)

# add all files and push them to exec
set (UTIL_TEST_LIB util_test.c ${ALL_FILES})
set (BLACKSHIP_TEST_SRC blackship_test.c ${ALL_FILES})
set (CMDS_TEST_SRC cmds_test.c ${ALL_FILES})
set (MEM_TEST_LIB mem_test.c ${ALL_FILES})
set (NET_TEST_LIB net_test.c ${ALL_FILES})

add_executable(util_test ${UTIL_TEST_LIB})
add_executable(blackship_test ${BLACKSHIP_TEST_SRC})
add_executable(cmds_test ${CMDS_TEST_SRC})
add_executable(mem_test ${MEM_TEST_LIB})
add_executable(net_test ${NET_TEST_LIB})

target_link_libraries(util_test cmocka)
target_link_libraries(blackship_test cmocka)
target_link_libraries(cmds_test cmocka)
target_link_libraries(mem_test  cmocka)
target_link_libraries(net_test  cmocka)

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

