# declare and set the flags

set(COMPILE_FLAGS "-std=gnu11 -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wmissing-prototypes -Wmissing-declarations -Winline -Wlong-long -Wuninitialized -Wconversion -Wstrict-prototypes -Werror -Wno-variadic-macros")
SET(CMAKE_C_FLAGS  "${COMPILE_FLAGS}")
# standard lib path to know where to search for dependency
set (LIB_PATH /usr/lib /usr/local/lib /usr/include)

