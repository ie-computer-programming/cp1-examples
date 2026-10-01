cmake_minimum_required(VERSION 3.20)

project(session09_examples
  DESCRIPTION "Session 9 — file input/output, streams, arguments and exit codes"
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
  01-read-lines
  02-bad-unchecked-fopen
  02-good-checked-fopen
  03-bad-feof-loop
  03-good-fgets-loop
  04-bad-scanf-unbounded
  04-good-fgets-bounded
  05-bad-leaky-error-path
  05-good-goto-cleanup
  06-write-and-check
  07-args-and-env
  08-streams-and-exit
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
  02-bad-unchecked-fopen
  04-bad-scanf-unbounded
)

if(NOT MSVC)
  foreach(example IN LISTS CP1_BROKEN)
    add_executable(${example}-asan ${example}.c)
    # Sanitizer flags are needed at BOTH compile and link time.
    target_compile_options(${example}-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
    target_link_options(${example}-asan PRIVATE -fsanitize=address)
  endforeach()
endif()
