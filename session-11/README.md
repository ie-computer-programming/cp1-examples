# Session 11 — preprocessor directives and macros

```text
01-expansion.c                what `cc -E` shows you; a macro is gone before compiling
02-include-guard.c            the same header included twice, harmlessly
include/point.h               guarded
include/point-unguarded.h     the same header without the guard (not built)
03-bad-unparenthesised.c      #define SQUARE(x) x*x  -- three different wrong answers
03-good-parenthesised.c       ((x) * (x)) -- why you need BOTH pairs
04-bad-double-eval.c          MAX(i++, j++) increments i twice
04-good-inline-function.c     static inline: arguments evaluated once
05-conditional-compilation.c  #if / #ifdef / #error, and NDEBUG
06-debug-log.c                the LOG macro Session 3 promised
07-stringify-and-paste.c      # and ##, and why XSTR exists
08-const-vs-define.c          #define vs const vs enum
```

Two targets are the *same source* built with a different flag — run the pair:

```text
05-conditional-compilation-verbose    -DVERBOSE -DNDEBUG
06-debug-log-release                  -DNDEBUG
```

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build

./run-all.sh          # every example, with its exit code
```

Nothing here crashes — this session's bugs all produce **wrong answers**, not
failures.

## What to look for

### 01 — read the expansion

```console
$ cc -E 01-expansion.c | tail
    printf("%s, %d\n", "Hello from " "CP1", 2026);
    printf("TWICE(21) = %d\n", ((21) + (21)));
```

The comments are gone, `GREETING` became two adjacent string literals (which
C concatenates), and `TWICE(21)` became its body. The compiler never hears
the word `GREETING`. **When a macro misbehaves, run `cc -E` instead of
guessing.**

### 03 — one missing pair of parentheses, three wrong answers

```console
$ ./build/03-bad-unparenthesised
SQUARE(5)       = 25    (fine, by luck)
SQUARE(2 + 3)   = 11    <-- expected 25
100 / SQUARE(5) = 100   <-- expected 4
HALF(4 + 4)     = 6     <-- expected 4
```

`SQUARE(2 + 3)` expands to `2 + 3 * 2 + 3`. The **inner** parentheses fix
that. `100 / SQUARE(5)` expands to `100 / 5 * 5`; only the **outer** pair
fixes that one. You need both, which is why the body is `((x) * (x))`.

Note the first line: with a plain argument the broken macro is right. That is
what makes this bug survive code review.

### 04 — the argument is pasted once per appearance

```console
$ ./build/04-bad-double-eval        $ ./build/04-good-inline-function
before : i = 5, j = 3               before : i = 5, j = 3
MAX(i++, j++) = 6                   max_int(i++, j++) = 5
after  : i = 7, j = 4               after  : i = 6, j = 4
```

`MAX(a,b)` mentions `a` twice, so `i++` runs twice. Look at the returned
values: **6 versus 5**. It is not only the side effect that differs — the
answer does.

Same shape as `assert(fclose(f) == 0)` from Session 3: a side effect hidden
inside something that is not a function call.

### 05 — code that never reaches the compiler

```console
$ ./build/05-conditional-compilation          $ ./build/05-...-verbose
platform      : Linux                         platform      : Linux
build         : quiet build                   build         : verbose build
__STDC_VERSION__ = 201710                     __STDC_VERSION__ = 201710
NDEBUG is not defined -- assert() is live     NDEBUG is defined -- assert() is gone
```

One source file, two programs. A false `#if` branch is deleted before
compilation, so it does not even have to be valid C — that is how a single
file supports macOS, Linux and Windows.

### 06 — the logger you never delete

```console
$ ./build/06-debug-log
[06-debug-log.c:48 divide] dividing 10 by 2
[06-debug-log.c:51 divide] refusing to divide by zero
10 / 2 = 5

$ ./build/06-debug-log-release
10 / 2 = 5
```

Same source. With `-DNDEBUG` every `LOG` expands to `((void)0)` **and the
`cp1_log` function itself is not compiled** — the whole mechanism disappears.

`((void)0)` rather than nothing so that `if (x) LOG("..."); else ...` still
parses.

### 07 — why `XSTR` exists

```console
STR(VERSION)   = VERSION    <-- the name
XSTR(VERSION)  = 7          <-- the value
XSTR(__LINE__) = 27
```

An argument of `#` or `##` is **not macro-expanded first**. Passing it
through a second macro forces the expansion before stringifying. This catches
everyone once.

### 08 — three ways to say 100

`#define` has no type, no scope and no debugger symbol, and it rewrites that
name everywhere after it appears — including inside headers you did not
write:

```c
#define count 10
struct bag { int count; };   /* error: expected identifier */
```

`enum { MAX = 100 }` is typed, scoped, a genuine compile-time constant, and
visible in the debugger. For integer constants it is almost always the right
choice, and almost nobody uses it.
