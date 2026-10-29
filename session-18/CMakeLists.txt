cmake_minimum_required(VERSION 3.20)

project(session18_examples
  DESCRIPTION "Session 18 — process management and IPC"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# fork, pipe, mmap, msgget and socketpair are POSIX, not ISO C.
set(CMAKE_C_EXTENSIONS ON)

if(WIN32)
  message(FATAL_ERROR "Session 18 uses fork(), pipes and POSIX IPC: build it on Linux, macOS or WSL.")
endif()

set(CP1_WARNINGS -Wall -Wextra)

# Explicit list, never file(GLOB): a glob silently misses new files until the
# next re-configure.
set(CP1_EXAMPLES
  01-fork-basics
  01-bad-no-flush
  02-bad-zombie
  02-good-wait
  03-bad-exec-falls-through
  03-good-exec
  04-pipe
  05-bad-pipe-hang
  06-shared-memory
  07-message-queue
  08-socketpair
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design.

# 06 stays at -O0: at -O2 the optimiser can fold the racy loop and hide the race
# (the same trap as Session 17's counter).
