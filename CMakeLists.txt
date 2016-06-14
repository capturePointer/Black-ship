# Project name
project(blackship C)
set(COMPILE_FLAGS "-c -std=gnu11 -g -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wmissing-prototypes -Wmissing-declarations -Winline -Wlong-long -Wuninitialized -Wconversion -Wstrict-prototypes -Werror -Wno-variadic-macros")

SET(CMAKE_C_FLAGS  "${COMPILE_FLAGS}")

cmake_policy(SET CMP0002 NEW)

# #WELCOME FUNCTIOn
function (welcome)
	message ("The build process is starting")
endfunction()

# ASSERTION MACRO FOR TESTING IF THE LIBARRY IS DEFINED
# ALSO IF IT"S DEFINED LIKED
function (assert TARGET LIB LIB_NAME)
	if (${LIB})
		message ("Library found: ${LIB_NAME}. LINKING")
		target_link_libraries(${TARGET} ${LIB})
	elseif (NOT ${LIB})
		message ("Assertion failed, libary not found ${LIB_NAME}")
	endif()
endfunction(assert)

# SPECIFY THE MINIMUM VERSION OF CMAKE
cmake_minimum_required(VERSION 2.6)
set (TARGET_MAIN src/blackship.c 
				src/cmds.c 
				src/opts.c 
				lib/err/err.c 
				lib/util/info.c 
				lib/util/mem.c)
set (LIB_PATH /usr/lib /usr/local/lib /usr/include)

add_executable(blackship ${TARGET_MAIN})

# LOOK FOR THE CMOCKA LIBARRY
find_library(CMOCKA_LIBRARY
	NAMES cmocka moka
	PATHS ${LIB_PATH})

# LOOK FOR THE ARGP LIBRARY
find_library(ARGP_LIBRARY
	Names argp arg
	PATH ${LIB_PATH})

if (ARGP_LIBRARY)
	message ( "DEFINED")
endif()

assert ("${TARGET}" ARGP_LIBRARY "ARGP")

# WELCOMING FUNCTION
welcome()
