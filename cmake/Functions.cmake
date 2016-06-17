include(CheckIncludeFile)
include (CheckLibraryExists)

function(check_libs)
	CHECK_INCLUDE_FILE("argp.h" ARGP_H)
	CHECK_INCLUDE_FILE("stdbool.h" STDBOOL_H)
	CHECK_INCLUDE_FILE("string.h" STRING_H)
	CHECK_INCLUDE_FILE("malloc.h" MALLOC_H)
	CHECK_INCLUDE_FILE("errno.h" ERRNO_H)
	CHECK_INCLUDE_FILE("stdint.h" STDINT_H)
	CHECK_INCLUDE_FILE("stdarg.h" STDARG_H)
	CHECK_INCLUDE_FILE("stddef.h" STDDEF_H)
	CHECK_INCLUDE_FILE("setjmp.h" SETJMP_H)
	set (ALL_H ${ARGP_H}
		${STDBOOL_H}
		${STRING_H}
		${ERRNO_H}
		${STDINT_H}
		${STDARG_H}
		${STDDEF_H}
		${SETJMP_H}
	)
	foreach(HEADER ${ALL_H})
		if(NOT ${HEADER})
			message(FATAL_ERROR "[!] PLEASE INSTALL MISSING HEADER")
		endif()
	endforeach(HEADER)
	message(STATUS "[*]Verifying external libraries")

	find_library(CMAKE_LIB
		NAMES cmocka cmoka.h
		HINTS "/usr/include /usr/lib /usr/local/lib /usr/local/include"
	)
	if(NOT ${CMOCKA_LIB})
		message(FATAL_ERROR "[!} PLEASE INSTALL MISSING LIBRARY")
	else()
		message(STATUS "[+] Cmoka library - found")
	endif()

endfunction(check_libs)
