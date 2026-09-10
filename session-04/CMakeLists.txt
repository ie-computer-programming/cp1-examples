cmake_minimum_required(VERSION 3.20)

# CXX is required because GTest is a C++ framework. Forgetting it gives
# "No CMAKE_CXX_COMPILER could be found".
project(session04_examples
  DESCRIPTION "Session 4-5 — unit testing with GTest, and the sanitizers"
  LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  set(CP1_WARNINGS /W4)
else()
  set(CP1_WARNINGS -Wall -Wextra)
endif()

# ---------------------------------------------------------------- library
add_library(stats src/stats.c)
target_include_directories(stats PUBLIC include)
target_compile_options(stats PRIVATE ${CP1_WARNINGS})

# ------------------------------------------------------------------ tests
find_package(GTest REQUIRED)
enable_testing()

add_executable(test_stats tests/test_stats.cpp)
target_link_libraries(test_stats PRIVATE stats GTest::gtest_main)
add_test(NAME stats_tests COMMAND test_stats)

# ------------------------------------------------------------- sanitizers
# Each demo is built TWICE from the same source: once plain, once instrumented.
# Run both -- the plain build is what makes the point, because it usually looks
# like nothing is wrong.
# A sanitizer must be given at BOTH compile and link time, hence each appears twice.
if(NOT MSVC)
  set(CP1_ASAN_DEMOS  01-buffer-overflow 02-use-after-free)
  set(CP1_UBSAN_DEMOS 03-signed-overflow)

  foreach(demo IN LISTS CP1_ASAN_DEMOS CP1_UBSAN_DEMOS)
    add_executable(${demo} sanitizers/${demo}.c)
    target_compile_options(${demo} PRIVATE -g ${CP1_WARNINGS})
  endforeach()

  foreach(demo IN LISTS CP1_ASAN_DEMOS)
    add_executable(${demo}-asan sanitizers/${demo}.c)
    target_compile_options(${demo}-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
    target_link_options(${demo}-asan PRIVATE -fsanitize=address)
  endforeach()

  foreach(demo IN LISTS CP1_UBSAN_DEMOS)
    add_executable(${demo}-ubsan sanitizers/${demo}.c)
    target_compile_options(${demo}-ubsan PRIVATE -fsanitize=undefined -g ${CP1_WARNINGS})
    target_link_options(${demo}-ubsan PRIVATE -fsanitize=undefined)
  endforeach()
endif()
