# This file contains only variables and set of definitions for the entire build process
# compile flags, include paths, etc.

# set the compile flags for every .c 
set(COMPILE_FLAGS "-std=gnu11 -I ../ -O -g -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wmissing-prototypes -Wmissing-declarations -Winline -Wuninitialized -Wstrict-prototypes -Werror -Wno-variadic-macros")
SET(CMAKE_C_FLAGS  "${COMPILE_FLAGS}")

