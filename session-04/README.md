# Sessions 4–5 — unit testing and the sanitizers

Two questions about your code, and a tool for each:

| Question | Tool |
|---|---|
| Does it give the **right answer**? | unit tests (GTest) |
| Is it doing something **illegal** along the way? | sanitizers, debugger |

Tests only answer the first. A program can pass every test and still be
corrupting memory — so you need both.

```text
include/stats.h          the library under test (no extern "C" -- like your cp1.h)
src/stats.c
tests/test_stats.cpp     GTest, C++, includes the C header correctly
sanitizers/              three deliberately buggy programs
```

## Build and run

```bash
conda activate programming
cmake -S . -B build
cmake --build build

cd build && ctest --output-on-failure     # run the tests
```

> **If `cmake` says `No CMAKE_CXX_COMPILER could be found`**, your environment
> predates GTest being added to the course. Update it:
> `conda env update -f environment.yml`

## GTest, and the one thing that will trip you up

GTest is a **C++** framework. Two consequences:

**1. The project must enable C++.** `project(... LANGUAGES C)` is not enough:

```cmake
project(session04_examples LANGUAGES C CXX)
```

**2. C++ mangles function names; C does not.** To allow overloading, a C++
compiler encodes argument types into the symbol name. Your C library exports
plain `stats_sum`; the C++ test looks for `stats_sum(int const*, unsigned long, int*)`.

Include a C header from C++ without saying so and the code compiles, then dies
at link time:

```console
undefined reference to `stats_sum(int const*, unsigned long, int*)'
```

The argument types in that message are the giveaway — that is a mangled name.

The fix, in the **test** file:

```cpp
#include <gtest/gtest.h>

extern "C" {
#include "stats.h"
}
```

There are two options in general, and which you use depends on who owns the
header:

| | When |
|---|---|
| `extern "C" { #include ... }` in the test | the header is not yours to change — **your HW1 `cp1.h` is a frozen contract, so this is the one you need** |
| `#ifdef __cplusplus` / `extern "C"` guards inside the header | the header is yours |

## Writing a test

```cpp
TEST(Sum, ReportsOverflowInsteadOfWrapping)
{
    const int values[] = {INT_MAX, 1};
    int out = 42;
    EXPECT_EQ(stats_sum(values, 2, &out), -1);
    EXPECT_EQ(out, 42) << "a failed call must leave the output untouched";
}
```

- `TEST(Suite, Name)` — the suite groups related cases
- `EXPECT_*` records a failure and **carries on**; `ASSERT_*` stops the test
  (use `ASSERT` when continuing would be meaningless, e.g. after checking a
  call succeeded)
- `<< "..."` adds a message shown only on failure

A failure tells you the values, not just that it failed:

```console
test_stats.cpp:91: Failure
Expected equality of these values:
  out
    Which is: 10
  11
```

See it yourself:

```bash
cmake -S . -B build-fail -DCMAKE_CXX_FLAGS=-DSHOW_FAILURE
cmake --build build-fail
./build-fail/test_stats
```

## The sanitizers

A sanitizer instruments your program at compile time, then checks every memory
access as it runs. Give the flag at **both** compile and link time.

```bash
cc -g -fsanitize=address    -o demo prog.c    # ASan  -- memory errors
cc -g -fsanitize=undefined  -o demo prog.c    # UBSan -- undefined behaviour
```

All three demos are already wired up in `CMakeLists.txt`:

```console
$ ./build/01-asan-buffer-overflow
ERROR: AddressSanitizer: stack-buffer-overflow
SUMMARY: ... 01-asan-buffer-overflow.c:15 in main

$ ./build/02-asan-use-after-free
ERROR: AddressSanitizer: heap-use-after-free
SUMMARY: ... 02-asan-use-after-free.c:20 in main

$ ./build/03-ubsan-signed-overflow
03-ubsan-signed-overflow.c:13:14: runtime error: signed integer overflow:
    2147483647 + 1 cannot be represented in type 'int'
```

Each maps onto something from HW1:

| Demo | HW1 connection |
|---|---|
| `01-asan-buffer-overflow` | why `cp1_copy_bounded` exists and `strcpy` is a defect |
| `02-asan-use-after-free` | freed memory usually still "works" — until it doesn't |
| `03-ubsan-signed-overflow` | exactly why `cp1_add_checked` must **detect** overflow, not compute it |

### Warnings, sanitizers, and what each catches

Building these prints two compiler warnings — GCC spotted the buffer overflow
and the use-after-free statically:

```text
warning: '__builtin_memcpy' writing 22 bytes into a region of size 8 [-Wstringop-overflow=]
warning: pointer 'values' used after 'free' [-Wuse-after-free]
```

That is the layering, and it is worth understanding:

| Tool | Catches | Cost |
|---|---|---|
| **Warnings** | mistakes visible in the code itself | free, every build |
| **Tests** | wrong answers | you write them |
| **Sanitizers** | illegal behaviour, only visible while running | ~2× slower |

The compiler caught these two because the sizes are literal constants. It
cannot catch the equivalent bug when the size arrives at run time — the leak in
`examples/session-03/06-bad-cleanup-leaky.c` produces **no warning at all**, and
only ASan finds it.

## Why not Valgrind?

The syllabus mentions Valgrind, and on Linux it does the same job. But
**Valgrind does not support Apple Silicon**, so most of this class cannot run
it. ASan is built into the compiler, works everywhere, and is faster.

Know that Valgrind exists and what it is for; use ASan.

## Debugging on macOS

Use VS Code's **Run and Debug** (`F5`) rather than `gdb` at the terminal.
macOS System Integrity Protection blocks debuggers from attaching, and the
codesigning workaround is fragile. VS Code uses `lldb` there automatically.

Set a breakpoint by clicking left of a line number, then `F5`. `F10` steps
over, `F11` steps into, and the Variables pane shows you what is in scope.

## Try it

1. Add a test for `stats_max` with a single element.
2. Break `stats_sum` on purpose — return `sum + 1` — and watch which tests fail.
3. Point GTest at **your own HW1 code** — see the next section.
4. Build your HW1 with `-fsanitize=address` and run its tests.

## Adding GTest to your HW1 repository

Your HW1 `CMakeLists.txt` has no GTest wiring — adding it is the exercise.
Three things are needed, and the third is easy to miss.

```cmake
# Append to the bottom of your HW1 CMakeLists.txt.
#
# QUIET + a guard, rather than REQUIRED: GTest is in your conda environment but
# NOT on the GitHub Actions runner that builds your repo. With REQUIRED, pushing
# this turns your CI tick red even though your code is fine.
find_package(GTest QUIET)

if(GTest_FOUND)
    enable_language(CXX)              # the project() line only declared C
    add_executable(test_gtest tests/test_gtest.cpp)
    target_link_libraries(test_gtest PRIVATE cp1 GTest::gtest_main)
    add_test(NAME gtest_all COMMAND test_gtest)
else()
    message(STATUS "GTest not found - skipping the GTest target")
endif()
```

```cpp
/* tests/test_gtest.cpp */
#include <gtest/gtest.h>

extern "C" {              /* cp1.h is C -- without this, the link fails */
#include "cp1.h"
}

TEST(Reverse, TwoChars) {
    char s[4] = {'a', 'b', '\0', 'Z'};
    cp1_reverse(s);
    EXPECT_STREQ(s, "ba");
    EXPECT_EQ(s[3], 'Z');     /* nothing past the '\0' may be touched */
}
```

Then, as usual:

```bash
cmake -S . -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

You are **not** required to commit this — it is practice, not graded. But with
the guard above it is safe to commit: locally your GTest tests run, and on the
CI runner they are skipped and the tick stays green.

> Remember `cp1.h` and `CMakeLists.txt` are part of the graded contract in
> spirit — the autograder compiles `src/*.c` directly and ignores your build
> files, so appending a test target cannot affect your score. Do not change
> `cp1.h` or the source file names.

