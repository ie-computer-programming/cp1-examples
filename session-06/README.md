# Session 6 — pointers

```text
01-address-and-value.c   value, address, and following a pointer
02-swap.c                pass by value vs. pass by address
03-array-walk.c          index vs. pointer traversal; the lost length
04-out-of-bounds.c       reading past the end, with and without ASan
```

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build

./build/01-address-and-value
./build/02-swap
./build/03-array-walk
```

## What to look for

### 01 — three ways to talk about one variable

```console
x   = 42                    the value
&x  = 0x7fff41d1cfdc        where x lives
p   = 0x7fff41d1cfdc        p holds that address
*p  = 42                    follow p, read there
&p  = 0x7fff41d1cfd0        p has its own address too
after *p = 99, x = 99       we changed x without naming it
```

The last line is the whole idea: `x` was modified through `p`, without `x`
appearing on the left of an assignment.

### 02 — why functions take pointers

```console
after swap_broken: x=1 y=2      nothing happened
after swap:        x=2 y=1
```

`swap_broken` received *copies*. It swapped them perfectly, then they were
discarded. This is why your HW1 functions take `int *quotient` and
`double *out` instead of returning the answer.

### 03 — the length does not survive the call

```console
in main, sizeof(a) = 20 bytes, so n = 5
inside the function, sizeof(a) = 8 (a pointer)
```

This file **warns on purpose**:

```text
warning: 'sizeof' on array function parameter 'a' will return size of 'const int *'
```

That warning is the lesson. Inside the function `a` is just an address, so
`sizeof` measures the pointer, not the array. **This is why every function
taking an array also takes an `n`.**

It also shows the two traversals producing identical output:

```c
for (size_t i = 0; i < n; i++)      { ... a[i] ... }
for (const int *p = a; p < a + n; p++) { ... *p ... }
```

### 04 — the bug you cannot see

```console
$ ./build/04-out-of-bounds
a[4]  = 50
a[10] = -1765628415          ← garbage, no error, exit code 0

$ ./build/04-out-of-bounds-asan
ERROR: AddressSanitizer: stack-buffer-overflow
SUMMARY: ... 04-out-of-bounds.c:14 in main
```

Same source, one flag apart. In Python this would have been an `IndexError`
on the spot; in C it is a plausible-looking number that quietly enters your
program.

## Try it

1. In `02-swap.c`, delete a `*` from `swap` and read the compiler error.
2. In `03-array-walk.c`, change `p < a + n` to `p <= a + n` and run the ASan
   build. What does it report, and why?
3. Write `int count_char(const char *s, char c)` that walks with a pointer
   rather than an index — no `[ ]` anywhere in the body.
4. Print `p`, `p + 1`, and `p + 2` as `%p` for an `int *` and then a
   `double *`. How many bytes apart are they, and why is it different?
