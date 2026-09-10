# Python → C: a bridge

You already know how to program. This is a lookup table for saying the same
things in C, plus the handful of places where C genuinely does not work like
Python. Keep it open for the first few weeks.

- [1. The big five differences](#1-the-big-five-differences)
- [2. Syntax lookup](#2-syntax-lookup)
- [3. Types](#3-types)
- [4. Arrays are not lists](#4-arrays-are-not-lists)
- [5. Strings](#5-strings)
- [6. Printing and reading input](#6-printing-and-reading-input)
- [7. Things with no Python equivalent](#7-things-with-no-python-equivalent)
- [8. Errors that will confuse you first](#8-errors-that-will-confuse-you-first)

## 1. The big five differences

| | Python | C |
|---|---|---|
| Running it | Interpreted — run the source | **Compiled** — build first, then run |
| Types | Inferred, can change at runtime | **Declared**, fixed at compile time |
| Numbers | `int` grows without limit | **Fixed width**, overflows silently |
| Memory | Garbage-collected for you | **Yours** to allocate and free |
| Mistakes | Exception, with a traceback | **Undefined behaviour** — possibly nothing |

The first two you notice immediately. The last three are what this course is
actually about.

## 2. Syntax lookup

| Python | C |
|---|---|
| `x = 5` | `int x = 5;` |
| `x = 5.0` | `double x = 5.0;` |
| `s = "hi"` | `char s[] = "hi";` |
| `xs = [1, 2, 3]` | `int xs[3] = {1, 2, 3};` |
| `len(xs)` | `sizeof xs / sizeof xs[0]` (arrays only) |
| `for x in xs:` | `for (int i = 0; i < n; i++) { ... xs[i] ... }` |
| `for i in range(n):` | `for (int i = 0; i < n; i++) { ... }` |
| `while cond:` | `while (cond) { ... }` |
| `if a and not b:` | `if (a && !b) { ... }` |
| `elif` | `else if` |
| `def f(a, b):` | `int f(int a, int b) { ... }` |
| `return x` | `return x;` |
| `print(x)` | `printf("%d\n", x);` |
| `# comment` | `/* comment */` or `// comment` |
| `True` / `False` | `1` / `0` (or `true`/`false` with `<stdbool.h>`) |
| `None` | `NULL` (for pointers) |
| `and` `or` `not` | `&&` `\|\|` `!` |

Two rules with no Python equivalent:

- **Every statement ends in a semicolon.**
- **Blocks use braces `{ }`, not indentation.** C ignores your whitespace
  entirely. Indent anyway — for the humans reading it.

## 3. Types

Every variable declares its type, and that type never changes.

```c
int      count   = 10;      /* whole number */
double   average = 3.5;     /* decimal */
char     initial = 'A';     /* ONE character, single quotes */
int     *pointer;           /* address of an int -- Session 6 */
```

Note `'A'` (single quotes, one character) versus `"A"` (double quotes, a
string). In Python both are `str`. In C they are completely different types.

### Numbers overflow

This is the difference that surprises Python programmers most.

```c
#include <limits.h>
int big = INT_MAX;      /* 2147483647 */
printf("%d\n", big + 1); /* -2147483648 -- wrapped around, no error */
```

Python would have happily given you `2147483648`. C gives you a negative
number and no warning at runtime. When exact width matters, use `<stdint.h>`:
`int8_t`, `uint32_t`, `int64_t`.

### Integer division

```c
printf("%d\n", 7 / 2);        /* 3   -- like Python's 7 // 2 */
printf("%f\n", 7.0 / 2);      /* 3.5 -- one operand is a double */
```

`7 / 2` in C is `3`, not `3.5`. If either operand is a floating-point type you
get floating-point division.

## 4. Arrays are not lists

```c
int a[5] = {1, 2, 3, 4, 5};
a[2];          /* 3 */
a[7] = 99;     /* compiles, runs, corrupts memory. No IndexError. */
```

An array has a **fixed size**, decided when you declare it. There is no
`append`, no slicing, no negative indexing, and **no bounds checking**.

C also does not remember how long an array is. Once you pass one to a
function, the length is gone — so you pass it separately:

```c
double average(const int *values, size_t count);   /* count travels with it */
```

Growable arrays are something you build yourself, with `malloc`/`realloc`
(Session 7) or as a linked list (Session 14).

## 5. Strings

C has no string type. A string is an array of `char` ending in a `'\0'` byte.

```c
char s[] = "hi";   /* actually {'h', 'i', '\0'} -- 3 bytes */
strlen(s);         /* 2 -- counts up to the '\0' */
sizeof(s);         /* 3 -- the storage, including the '\0' */
```

| Python | C | Needs |
|---|---|---|
| `len(s)` | `strlen(s)` | `<string.h>` |
| `a + b` | `strcat(dest, src)` | `<string.h>` |
| `a == b` | `strcmp(a, b) == 0` | `<string.h>` |
| `a in b` | `strstr(b, a) != NULL` | `<string.h>` |
| `s.copy()` | `strcpy(dest, src)` | `<string.h>` |

**`==` on two C strings compares addresses, not contents.** It will compile,
run, and quietly give you the wrong answer. Use `strcmp`.

## 6. Printing and reading input

`print` inspects the value and works out how to display it. `printf` does not
— you state the type and it believes you.

```c
printf("%d\n", 42);           /* int */
printf("%f\n", 3.14);         /* double */
printf("%s\n", "text");       /* string */
printf("%c\n", 'A');          /* char */
printf("%zu\n", sizeof(int)); /* size_t */
```

| Specifier | Type |
|---|---|
| `%d` | `int` |
| `%u` | `unsigned int` |
| `%ld` | `long` |
| `%zu` | `size_t` |
| `%f` | `double` |
| `%c` | `char` |
| `%s` | `char *` |
| `%p` | pointer |
| `%x` | hexadecimal |
| `%%` | a literal `%` |

Getting this wrong is undefined behaviour, not an error — which is one more
reason to build with `-Wall -Wextra -Werror`, which catches most mismatches.

For input, prefer `fgets` over `scanf` while you are learning; `scanf` has
sharp edges around whitespace and buffer sizes.

## 7. Things with no Python equivalent

You will meet each of these in its own session. Nothing to memorise now —
just know they are coming, and that they have no counterpart in Python.

| Concept | What it is | Session |
|---|---|---|
| **Header files** | Declarations split into `.h`, definitions into `.c` | 2 |
| **The build** | Preprocess → compile → assemble → link | 1–2 |
| **Manual memory** | `malloc` what you need, `free` it yourself | 7 |
| **Pointers** | A variable holding a memory address | 6 |
| **`struct`** | Like a class with only data, no methods | 8 |
| **Undefined behaviour** | Breaking a rule with no defined consequence | 1, 5 |

## 8. Errors that will confuse you first

| Message | What it usually means |
|---|---|
| `expected ';' before ...` | Missing semicolon — often on the line *above* the one reported |
| `implicit declaration of function 'printf'` | Missing `#include <stdio.h>` |
| `undefined reference to 'foo'` | Compiled fine, but the **linker** cannot find the body of `foo` |
| `Segmentation fault` | You touched memory you do not own — usually a bad pointer or an out-of-range index |
| `warning: unused variable 'x'` | Exactly that — with `-Werror` it stops the build, deliberately |
| `format '%d' expects ... but argument has type ...` | Your `printf` specifier does not match the value |

Two habits that will save you hours:

1. **Fix the first error, then recompile.** A wall of 40 errors is usually one
   real mistake plus 39 consequences.
2. **A `Segmentation fault` is not a compiler problem.** It happens at run
   time, and Session 5 gives you the tools to find where.
