cmake_minimum_required(VERSION 3.20)

project(session08_examples
  DESCRIPTION "Session 8 — structures and unions"
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
  01-struct-basics
  02-typedef-and-arrow
  03-bad-pass-by-value
  03-good-pass-by-pointer
  04-padding-and-alignment
  05-struct-on-heap
  06-bad-shallow-copy
  06-good-deep-copy
  07-union-basics
  08-bad-union-untagged
  08-good-tagged-union
  09-aos-vs-soa
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design and would refuse to build.

# The broken examples, rebuilt with AddressSanitizer. Run the pair and
# compare: the plain build hides the bug, the -asan build names it.
set(CP1_BROKEN
  06-bad-shallow-copy
)

if(NOT MSVC)
  foreach(example IN LISTS CP1_BROKEN)
    add_executable(${example}-asan ${example}.c)
    # Sanitizer flags are needed at BOTH compile and link time.
    target_compile_options(${example}-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
    target_link_options(${example}-asan PRIVATE -fsanitize=address)
  endforeach()
endif()

# 09 measures cache behaviour, so it needs optimisation to say anything
# meaningful. Everything else stays at the default -O0 so the debugger and
# the sanitizers show what the source says.
if(NOT MSVC)
  target_compile_options(09-aos-vs-soa PRIVATE -O2)
endif()
