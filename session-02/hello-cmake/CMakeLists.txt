cmake_minimum_required(VERSION 3.20)

project(hello_cmake
  VERSION 1.0.0
  DESCRIPTION "Session 2 — the smallest useful CMake project"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# conda-forge ships MSVC on Windows and GCC/Clang elsewhere, so the flags differ.
if(MSVC)
  set(CP1_WARNINGS /W4 /WX)
else()
  set(CP1_WARNINGS -Wall -Wextra -Werror)
endif()

# A library: reusable code, no main().
add_library(greet src/greet.c)
target_include_directories(greet PUBLIC include)
target_compile_options(greet PRIVATE ${CP1_WARNINGS})

# An executable: has main(), links the library.
add_executable(hello src/main.c)
target_link_libraries(hello PRIVATE greet)
target_compile_options(hello PRIVATE ${CP1_WARNINGS})
