cmake_minimum_required(VERSION 3.20)

project(session17_examples
  DESCRIPTION "Session 17 — advanced thread management"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# usleep, clock_gettime, sem_open and alarm are POSIX, not ISO C.
set(CMAKE_C_EXTENSIONS ON)

if(MSVC)
  set(CP1_WARNINGS /W4)
else()
  set(CP1_WARNINGS -Wall -Wextra)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Explicit list, never file(GLOB): a glob silently misses new files until the
# next re-configure.
set(CP1_EXAMPLES
  01-fixing-the-counter
  02-bad-check-then-act
  02-good-whole-section
  03-bad-deadlock
  03-good-lock-order
  04-semaphore-pool
  05-barrier
  07-atomics
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
  target_include_directories(${example} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(${example} PRIVATE Threads::Threads)
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design.

if(NOT MSVC)
  # 01 stays at -O0 on purpose: at -O2 gcc folds the racy loop into one
  # `counter += 1000000`, so the race no longer loses anything. It is still UB.

  # ThreadSanitizer: reports the data race in the racy counter, and is silent
  # once the counter is protected. It also reports the lock-order inversion.
  foreach(example IN ITEMS 01-fixing-the-counter 03-bad-deadlock)
    add_executable(${example}-tsan ${example}.c)
    target_compile_options(${example}-tsan PRIVATE -fsanitize=thread -g ${CP1_WARNINGS})
    target_link_options(${example}-tsan PRIVATE -fsanitize=thread)
    target_link_libraries(${example}-tsan PRIVATE Threads::Threads)
  endforeach()
  target_compile_definitions(03-bad-deadlock-tsan PRIVATE SEQUENTIAL)
endif()

# OpenMP is optional: Apple's clang ships without it. Skip the example
# rather than break the whole build.
find_package(OpenMP)
if(OpenMP_C_FOUND)
  add_executable(06-openmp 06-openmp.c)
  target_compile_options(06-openmp PRIVATE ${CP1_WARNINGS} -O2)
  target_link_libraries(06-openmp PRIVATE OpenMP::OpenMP_C)
else()
  message(STATUS "OpenMP not found -- skipping 06-openmp")
endif()
