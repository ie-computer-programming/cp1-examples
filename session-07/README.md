# Session 7 — dynamic memory and memory management

```text
01-stack-vs-heap.c          where memory comes from; a runtime-sized block
02-malloc-free.c            malloc / calloc / free, and the sizeof *p idiom
03-bad-dangling-return.c    returning the address of a local
03-good-heap-return.c       the heap outlives the function; who owns the block
04-bad-leak.c               100 allocations, 0 frees, exit code 0
04-good-freed.c             one free per malloc
05-bad-use-after-free.c     read, write and free again after free
05-good-null-after-free.c   free, then NULL
06-bad-off-by-one.c         malloc(strlen(s)) -- the missing + 1
06-good-room-for-nul.c      malloc(strlen(s) + 1) + memcpy
07-bad-strcpy.c             strcpy has no size and cannot fail
07-good-snprintf.c          snprintf always takes a size, always terminates
08-realloc-grow.c           the growable array Python calls list
```

Every `*-bad-*` file is also built with AddressSanitizer as `<name>-asan`.
**Run the pair.** The plain build hides the bug; the `-asan` build names it,
with the line number.

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build

./run-all.sh          # every example, with its exit code
```

Several of these crash or abort on purpose, so `run-all.sh` does not stop on
failure.

## What to look for

### 03 — a pointer to a dead stack frame

```console
$ ./build/03-bad-dangling-return
Segmentation fault
[exit code: 139]
```

The compiler already told you, before you ran anything:

```text
warning: function returns address of local variable [-Wreturn-local-addr]
```

`03-good-heap-return.c` fixes it by allocating on the heap — and documents in
a comment that **the caller must free it**. C has no syntax for ownership, so
the comment is the only place it can live.

### 04 — the silent one

```console
$ ./build/04-bad-leak
total = 4950
exit code will be 0 -- the leak is silent
[exit code: 0]
```

Nothing is wrong, as far as the program is concerned. Now ask ASan:

```console
$ ./build/04-bad-leak-asan
ERROR: LeakSanitizer: detected memory leaks
Direct leak of 25600 byte(s) in 100 object(s) allocated from:
    #1 ... in main .../04-bad-leak.c:20
SUMMARY: AddressSanitizer: 25600 byte(s) leaked in 100 allocation(s).
```

25.6 KB and the exact line. This is the bug class Python's garbage collector
made impossible for you, and it is back.

### 05 — free does not change your pointer

```console
$ ./build/05-bad-use-after-free
free(): double free detected in tcache 2
Aborted
[exit code: 134]
```

glibc happened to notice the *double free*. It said nothing about the read
and the write that came first. ASan sees all three:

```text
ERROR: AddressSanitizer: heap-use-after-free on address 0x...
READ of size 4 at 0x... thread T0
    #0 ... in main .../05-bad-use-after-free.c:29
```

### 06 — the off-by-one that looks like it works

```console
$ ./build/06-bad-off-by-one
copy = hello
[exit code: 0]
```

Correct output, exit code 0, and a heap overflow on every run:

```text
ERROR: AddressSanitizer: heap-buffer-overflow
WRITE of size 6 at 0x... thread T0
0x... is located 0 bytes after 5-byte region [0x...10,0x...15)
```

"0 bytes after a 5-byte region" is the `'\0'` landing one past the end.
`strlen("hello")` is 5; the copy needs 6.

### 07 — the function that cannot fail safely

```console
$ ./build/07-bad-strcpy
Segmentation fault
[exit code: 139]
```

Again the compiler warned first:

```text
warning: 'strcpy' writing 36 bytes into a region of size 8 overflows the destination
```

Compare with `07-good-snprintf.c`, which takes the size, terminates, and
tells you whether it had to truncate:

```console
$ ./build/07-good-snprintf
small  = ./build
after  = SAFE  <-- untouched
truncated: needed 25 bytes, had 8

$ ./build/07-good-snprintf hi
small  = hi
fitted with room to spare
```

### 08 — what `list.append` was doing

```console
grew to capacity 4
grew to capacity 8
grew to capacity 16
10 values: 1 2 3 4 5 6 7 8 9 10
```

Note the `tmp` pointer in the realloc call. Writing `a = realloc(a, ...)`
directly is a leak waiting to happen: if realloc fails it returns `NULL`, the
original block is still alive, and you have just overwritten your only
pointer to it.

## Why no Valgrind?

Valgrind does not support Apple Silicon, so most of this class cannot run it.
`-fsanitize=address` works everywhere, catches the same bug classes, and is
faster. Valgrind is worth knowing by name; it is not worth your afternoon.
