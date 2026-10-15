cmake_minimum_required(VERSION 3.20)

project(session15_examples
  DESCRIPTION "Session 15 — recursion and backtracking"
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
  01-factorial-frames
  02-bad-no-base-case
  02-good-base-case
  03-fibonacci
  04-hanoi
  05-permutations
  06-n-queens
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()

# -Werror stays OFF here on purpose: 02-bad is wrong by design.

if(NOT MSVC)
  # Timings at -O0 measure the compiler's laziness, not the algorithm.
  target_compile_options(03-fibonacci PRIVATE -O2)
  target_compile_options(06-n-queens PRIVATE -O2)

  # The runaway recursion, rebuilt with AddressSanitizer: it names the overflow.
  add_executable(02-bad-no-base-case-asan 02-bad-no-base-case.c)
  target_compile_options(02-bad-no-base-case-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
  target_link_options(02-bad-no-base-case-asan PRIVATE -fsanitize=address)
endif()
