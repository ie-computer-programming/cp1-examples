# Session 3 — error handling and assertions

One runnable program per pattern from the session, in **bad / good pairs** so
you can see the difference rather than take it on trust.

```text
01-bad-unchecked-return.c     01-good-checked-return.c      checking return values
02-bad-errno-silent.c         02-good-errno-reported.c      errno / perror / strerror
03-bad-assert-missing.c       03-good-assert-present.c      assert() on a contract
04-trap-assert-side-effect.c                                the NDEBUG trap
05-static-assert.c                                          compile-time checks
06-bad-cleanup-leaky.c        06-good-cleanup-goto.c        goto cleanup
```

## Build and run

```bash
conda activate programming
cmake -S . -B build
cmake --build build

./build/01-good-checked-return          # run one
./run-all.sh                            # run all, with exit codes
```

Several of these **fail on purpose** — that is the lesson. `run-all.sh` keeps
going and prints each exit code.

> `-Werror` is deliberately *not* enabled in this directory (see
> `CMakeLists.txt`). Half these programs are wrong on purpose and the compiler
> would rightly refuse to build them.

## What to look for

### 01 — check your return values

```console
$ ./build/01-bad-unchecked-return
Segmentation fault                              [exit code: 139]

$ ./build/01-good-checked-return
could not open does-not-exist.txt               [exit code: 1]
```

Same failure, same cause. One crashes with no explanation; the other says what
went wrong and exits cleanly.

### 02 — `errno` says *why*

```console
$ ./build/02-bad-errno-silent
error

$ ./build/02-good-errno-reported
fopen: No such file or directory
could not open does-not-exist.txt: No such file or directory (errno 2)
```

Note that the good version saves `errno` into a local **immediately**. `perror`
and `fprintf` are library calls and are allowed to overwrite it.

### 03 — `assert` turns a wrong answer into a stopped program

```console
$ ./build/03-bad-assert-missing
average of 3: 2.000000
average of 0: -nan                              [exit code: 0]

$ ./build/03-good-assert-present
average of 3: 2.000000
average of 0: 03-good-assert-present.c:10: average:
              Assertion `count > 0' failed.
Aborted                                         [exit code: 134]
```

The bad one is the dangerous case: **exit code 0**. As far as anything
downstream is concerned, that run succeeded — and `-nan` is now in your data.

### 04 — the `NDEBUG` trap

Build it both ways and compare:

```console
$ cmake -S . -B build && cmake --build build
$ ./build/04-trap-assert-side-effect
next_value() was called 1 time(s)
built without NDEBUG -> assertions active

$ cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
$ cmake --build build-release
$ ./build-release/04-trap-assert-side-effect
next_value() was called 0 time(s)
built with NDEBUG -> assertions removed
```

The function was **never called** in the release build, because the whole
`assert(...)` line — expression included — was compiled out. Never put a side
effect inside an assertion.

### 05 — `static_assert` fails before the program exists

```console
$ cmake --build build --target 05-static-assert     # builds fine
$ cc -std=c17 -DSHOW_FAILURE -c 05-static-assert.c -o /dev/null
05-static-assert.c:16:1: error: static assertion failed:
    "deliberately wrong: int is not 2 bytes here"
```

No binary is produced, so this class of bug can never reach a user.

### 06 — cleanup on the error path

Both versions print the same thing, which is exactly the problem — a leak is
invisible from the outside:

```console
$ ./build/06-bad-cleanup-leaky  build/empty.txt
build/empty.txt: empty or unreadable            [exit code: 1]
$ ./build/06-good-cleanup-goto  build/empty.txt
build/empty.txt: empty or unreadable            [exit code: 1]
```

You need a tool to see the difference. This is a preview of Session 5:

```console
$ cc -std=c17 -g -fsanitize=address -o leaky 06-bad-cleanup-leaky.c
$ ./leaky empty.txt
Direct leak of 4096 byte(s) in 1 object(s) allocated from: ...
SUMMARY: AddressSanitizer: 4096 byte(s) leaked in 1 allocation(s).

$ cc -std=c17 -g -fsanitize=address -o clean 06-good-cleanup-goto.c
$ ./clean empty.txt
(nothing — no leak)
```

## How this directory builds many programs at once

Each `.c` file has its own `main()`, so each becomes **its own executable**.
Rather than repeating `add_executable` ten times, list the programs once and
loop:

```cmake
set(CP1_EXAMPLES
  01-bad-unchecked-return
  01-good-checked-return
  # ...
)

foreach(example IN LISTS CP1_EXAMPLES)
  add_executable(${example} ${example}.c)
  target_compile_options(${example} PRIVATE ${CP1_WARNINGS})
endforeach()
```

Adding a program is then one line in the list.

**Why not `file(GLOB *.c)`?** Because CMake would only notice new files the
next time it happens to re-configure — so a file you just added silently does
not build until someone works out why. An explicit list makes adding a program
a visible, reviewable change. Globbing is convenient right up to the point it
wastes an hour of your life.

**The single-`main()` rule.** If you instead added all ten `.c` files to one
executable, the linker would fail with *"multiple definition of `main`"*. One
`main` per program, always.
