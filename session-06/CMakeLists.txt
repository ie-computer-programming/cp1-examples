cmake_minimum_required(VERSION 3.20)

project(session06_examples
  DESCRIPTION "Session 6 — pointers"
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

set(CP1_EXAMPLES
  01-address-and-value
  02-swap
  03-array-walk
  04-out-of-bounds
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()

# The same out-of-bounds program, instrumented. Compare the two.
if(NOT MSVC)
  add_executable(04-out-of-bounds-asan 04-out-of-bounds.c)
  target_compile_options(04-out-of-bounds-asan PRIVATE -fsanitize=address -g ${CP1_WARNINGS})
  target_link_options(04-out-of-bounds-asan PRIVATE -fsanitize=address)
endif()
