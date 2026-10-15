cmake_minimum_required(VERSION 3.20)

project(session13_examples
  DESCRIPTION "Session 13 — enumerations and typedef"
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
  01-enum-basics
  02-bad-enum-is-int
  03-x-macro
  04-bad-typedef-alias
  04-good-struct-wrapper
  05-function-pointer
  06-bad-typedef-pointer
  06-good-typedef-struct
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design and would refuse to build.
