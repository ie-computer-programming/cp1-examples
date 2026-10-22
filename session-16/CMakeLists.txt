cmake_minimum_required(VERSION 3.20)

project(session16_examples
  DESCRIPTION "Session 16 — thread concepts and lifecycle"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# usleep and clock_gettime are POSIX, not ISO C.
set(CMAKE_C_EXTENSIONS ON)

if(MSVC)
  set(CP1_WARNINGS /W4)
else()
  set(CP1_WARNINGS -Wall -Wextra)
endif()

# Adds -pthread at compile AND link time, portably.
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Explicit list, never file(GLOB): a glob silently misses new files until the
# next re-configure.
set(CP1_EXAMPLES
  01-hello-threads
  02-bad-no-join
  02-good-join
  03-bad-loop-variable
  03-good-per-thread-args
  04-parallel-sum
  05-race-preview
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
  target_link_libraries(${example} PRIVATE Threads::Threads)
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design.

if(NOT MSVC)
  target_compile_options(04-parallel-sum PRIVATE -O2)

  # ThreadSanitizer: finds data races, even when the answer happens to be right.
  # It cannot be combined with AddressSanitizer in the same binary.
  foreach(example IN ITEMS 03-bad-loop-variable 05-race-preview)
    add_executable(${example}-tsan ${example}.c)
    target_compile_options(${example}-tsan PRIVATE -fsanitize=thread -g ${CP1_WARNINGS})
    target_link_options(${example}-tsan PRIVATE -fsanitize=thread)
    target_link_libraries(${example}-tsan PRIVATE Threads::Threads)
  endforeach()
endif()
