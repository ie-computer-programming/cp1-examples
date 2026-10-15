cmake_minimum_required(VERSION 3.20)

project(session14_examples
  DESCRIPTION "Session 14 — linked lists"
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
  01-node-basics
  02-bad-push-by-value
  02-good-push-pointer-to-pointer
  03-bad-free-while-walking
  03-good-free-list
  04-remove
  05-reverse
  06-array-vs-list
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design and would refuse to build.

if(NOT MSVC)
  # A benchmark at -O0 measures the compiler's laziness, not the data structure.
  target_compile_options(06-array-vs-list PRIVATE -O2)

  # The broken examples, rebuilt with AddressSanitizer: the plain build hides
  # the bug, the -asan build names it.
  foreach(example IN ITEMS 02-bad-push-by-value 03-bad-free-while-walking)
    add_executable(${example}-asan ${example}.c)
    target_compile_options(${example}-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
    target_link_options(${example}-asan PRIVATE -fsanitize=address)
  endforeach()
endif()
