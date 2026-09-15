cmake_minimum_required(VERSION 3.20)

project(session07_examples
  DESCRIPTION "Session 7 — dynamic memory and memory management"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
  set(CP1_WARNINGS /W4)
else()
  set(CP1_WARNINGS -Wall -Wextra)
endif()

# Explicit list, never file(GLOB): a glob silently misses new files until the
# next re-configure.
set(CP1_EXAMPLES
  01-stack-vs-heap
  02-malloc-free
  03-bad-dangling-return
  03-good-heap-return
  04-bad-leak
  04-good-freed
  05-bad-use-after-free
  05-good-null-after-free
  06-bad-off-by-one
  06-good-room-for-nul
  07-bad-strcpy
  07-good-snprintf
  08-realloc-grow
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design and would refuse to build.

# Every broken example, rebuilt with AddressSanitizer. Run the pair and
# compare: the plain build hides the bug, the -asan build names it.
set(CP1_BROKEN
  03-bad-dangling-return
  04-bad-leak
  05-bad-use-after-free
  06-bad-off-by-one
  07-bad-strcpy
)

if(NOT MSVC)
  foreach(example IN LISTS CP1_BROKEN)
    add_executable(${example}-asan ${example}.c)
    # Sanitizer flags are needed at BOTH compile and link time.
    target_compile_options(${example}-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
    target_link_options(${example}-asan PRIVATE -fsanitize=address)
  endforeach()
endif()
