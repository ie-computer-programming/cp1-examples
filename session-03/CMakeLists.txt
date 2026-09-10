cmake_minimum_required(VERSION 3.20)

project(session03_examples
  DESCRIPTION "Session 3 — error handling and assertions, one program per pattern"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# NOTE: -Werror is deliberately absent here. Half these programs are wrong on
# purpose, and the compiler would rightly refuse to build them.
if(MSVC)
  set(CP1_WARNINGS /W4)
else()
  set(CP1_WARNINGS -Wall -Wextra)
endif()

# One executable per file. Listed explicitly rather than file(GLOB ...) so that
# adding a file is a visible change and CMake never silently misses one.
set(CP1_EXAMPLES
  01-bad-unchecked-return
  01-good-checked-return
  02-bad-errno-silent
  02-good-errno-reported
  03-bad-assert-missing
  03-good-assert-present
  04-trap-assert-side-effect
  05-static-assert
  06-bad-cleanup-leaky
  06-good-cleanup-goto
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()
