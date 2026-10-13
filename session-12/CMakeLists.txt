cmake_minimum_required(VERSION 3.20)

project(session12_examples
  DESCRIPTION "Session 12 — bit manipulation"
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
  01-operators
  02-masks
  03-bad-precedence
  03-good-precedence
  04-bad-shift
  04-good-shift
  05-twos-complement
  06-enum-flags
  07-morton
  08-bad-rsqrt
  08-good-rsqrt
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
  target_include_directories(${example} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design and would refuse to build.

if(NOT MSVC)
  # sqrtf lives in libm on Linux.
  target_link_libraries(08-bad-rsqrt PRIVATE m)
  target_link_libraries(08-good-rsqrt PRIVATE m)

  # Strict aliasing is only exploited, and only warned about, when optimising.
  target_compile_options(08-bad-rsqrt PRIVATE -O2)
  target_compile_options(08-good-rsqrt PRIVATE -O2)

  # Same source + UndefinedBehaviorSanitizer: the plain build hides the bug.
  add_executable(04-bad-shift-ubsan 04-bad-shift.c)
  target_compile_options(04-bad-shift-ubsan PRIVATE -fsanitize=undefined -g ${CP1_WARNINGS})
  target_link_options(04-bad-shift-ubsan PRIVATE -fsanitize=undefined)

  add_executable(08-bad-rsqrt-asan 08-bad-rsqrt.c)
  target_compile_options(08-bad-rsqrt-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
  target_link_options(08-bad-rsqrt-asan PRIVATE -fsanitize=address)
  target_link_libraries(08-bad-rsqrt-asan PRIVATE m)
endif()
