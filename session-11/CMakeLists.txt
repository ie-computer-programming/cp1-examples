cmake_minimum_required(VERSION 3.20)

project(session11_examples
  DESCRIPTION "Session 11 — preprocessor directives and macros"
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
  01-expansion
  02-include-guard
  03-bad-unparenthesised
  03-good-parenthesised
  04-bad-double-eval
  04-good-inline-function
  05-conditional-compilation
  06-debug-log
  07-stringify-and-paste
  08-const-vs-define
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
  target_include_directories(${example} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endforeach()

# -Werror stays OFF here on purpose: several of these programs are wrong by
# design and would refuse to build.

# Second builds that flip a preprocessor switch. Run the pair and compare --
# same source, different text reaching the compiler.
add_executable(05-conditional-compilation-verbose 05-conditional-compilation.c)
target_compile_options(05-conditional-compilation-verbose PRIVATE ${CP1_WARNINGS} -DVERBOSE -DNDEBUG)

add_executable(06-debug-log-release 06-debug-log.c)
target_compile_options(06-debug-log-release PRIVATE ${CP1_WARNINGS} -DNDEBUG)
