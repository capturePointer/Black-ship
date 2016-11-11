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
	CHECK_INCLUDE_FILE("argz.h" ARGZ_H)
	CHECK_INCLUDE_FILE("stdlib.h" STDLIB_H)
	CHECK_INCLUDE_FILE("fcntl.h" FCNTL_H)
	CHECK_INCLUDE_FILE("signal.h" SIGNAL_H)
	CHECK_INCLUDE_FILE("limits.h" LIMITS_H)
	CHECK_INCLUDE_FILE("unistd.h" UNISTD_H)
	CHECK_INCLUDE_FILE("arpa/inet.h" ARPAINET_H)
	CHECK_INCLUDE_FILE("sys/types.h" SYSTYPES_H)
	CHECK_INCLUDE_FILE("inttypes.h" INTTYPES_H)
	CHECK_INCLUDE_FILE("stdio.h" STDIO_H)
	CHECK_INCLUDE_FILE("sys/socket.h" SOCKET_H)
	CHECK_INCLUDE_FILE("netinet/tcp.h" TCP_H)
	CHECK_INCLUDE_FILE("netinet/ip.h" IP_H)
	CHECK_INCLUDE_FILE("netinet/in.h" NETINET_IN_H)
	CHECK_INCLUDE_FILE("ifaddrs.h" IFADDRS_H)
	CHECK_INCLUDE_FILE("sys/wait.h" SYSWAIT_H)
	set (ALL_H ${ARGP_H}
		${STDBOOL_H}
		${STRING_H}
		${ERRNO_H}
		${STDINT_H}
		${STDARG_H}
		${STDDEF_H}
		${SETJMP_H}
		${ARGZ_H}
		${STDLIB_H}
		${FCNTL_H}
		${SIGNAL_H}
		${LIMITS_H}
		${UNISTD_H}
		${ARPAINET_H}
		${SYSTYPES_H}
		${INTTYPES_H}
		${STDIO_H}
		${SOCKET_H}
		${TCP_H}
		${IP_H}
		${IFADDRS_H}
		${SYSWAIT_H}
		${NETINET_IN_H}
	)
	foreach(HEADER ${ALL_H})
		if(NOT ${HEADER})
			message(FATAL_ERROR "[!] PLEASE INSTALL MISSING HEADER")
		endif()
	endforeach(HEADER)

endfunction(check_libs)
