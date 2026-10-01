# Session 9 — file input/output, streams, arguments and exit codes

```text
01-read-lines.c             the fgets loop, and stripping the '\n'
02-bad-unchecked-fopen.c    fopen returns NULL and the program carries on
02-good-checked-fopen.c     check it, then perror the path
03-bad-feof-loop.c          while (!feof(f)) -- one iteration too many
03-good-fgets-loop.c        loop on what the read returns
04-bad-scanf-unbounded.c    fscanf("%s") has no size, so it cannot be safe
04-good-fgets-bounded.c     a width, or better, fgets(buf, sizeof buf, f)
05-bad-leaky-error-path.c   an early return that forgets to fclose
05-good-goto-cleanup.c      Session 3's cleanup pattern, with files
06-write-and-check.c        buffering, and why fclose can fail
07-args-and-env.c           argc / argv / getenv, and text is not a number
08-streams-and-exit.c       stdin, stdout, stderr, redirection, exit codes
```

`02-bad` and `04-bad` are also built with AddressSanitizer as `<name>-asan`.
**Run the pair.**

Two small data files come with these: `sample.txt` (three lines) and
`long-word.txt` (one 34-character word).

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build

./run-all.sh          # every example, with its exit code
```

Run it from **this** directory — the examples open `sample.txt` by a relative
path. Two of them crash on purpose, so `run-all.sh` does not stop on failure.

## What to look for

### 02 — `fopen` fails by returning `NULL`

```console
$ ./build/02-bad-unchecked-fopen
Segmentation fault                                   [exit code: 139]
```

The crash is in `fgets`, not in `fopen`, and nothing says which file was
missing. ASan at least names the line:

```console
ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000
Hint: address points to the zero page.
    #2 ... in main .../02-bad-unchecked-fopen.c:15
```

`02-good` prints `does-not-exist.txt: No such file or directory` and exits 1.
Two lines of code, and the difference between a stack trace and an answer.

### 03 — the `feof` trap

```console
$ ./build/03-bad-feof-loop          $ ./build/03-good-fgets-loop
 1: alpha                            1: alpha
 2: beta                             2: beta
 3: gamma                            3: gamma
 4: gamma   <-- from a 3-line file   clean end of file after 3 lines
[exit code: 0]
```

`feof` reports whether a **previous** read hit the end. On the last pass
`fgets` fails, returns `NULL`, and leaves the buffer holding the previous
line — which then gets printed again. Loop on the return value instead.

### 04 — a conversion with no size

```console
$ ./build/04-bad-scanf-unbounded
Segmentation fault                                   [exit code: 139]

$ ./build/04-bad-scanf-unbounded-asan
ERROR: AddressSanitizer: stack-buffer-overflow
WRITE of size 35 ...
  This frame has 1 object(s):
    [32, 40) 'word'  <== Memory access at offset 40 overflows this variable
```

34 characters plus a terminator into an 8-byte buffer. ASan names the
variable. This is `strcpy` from Session 7 in different clothing.

### 05 — no `with` means every `return` is a leak

```console
$ ./build/05-bad-leaky-error-path
sample.txt: Too many open files
iteration 62: could not open sample.txt any more     [exit code: 0]

$ ./build/05-good-goto-cleanup
2000 iterations, every file closed on every path     [exit code: 0]
```

Both call the same function in a loop. The bad one returns early when a
second file fails to open, leaving the first one open every time; after 62
iterations there are no descriptors left and something **unrelated** starts
failing. Note the exit code: still 0.

> Both programs lower their own descriptor limit to 64 with `setrlimit` so the
> demo is quick and identical everywhere. That call is scaffolding, not the
> lesson.

### 06 — your writes are not on the disk yet

`fprintf` fills a buffer; the C library flushes it when the buffer fills, on
`fflush`, or on `fclose`. So a write error — a full disk, a disconnected
share — is usually reported by **`fclose`**, long after the `fprintf` that
produced the data. Check it on any file you wrote.

### 08 — stdout is buffered, stderr is not

Run it straight to a terminal and the lines appear in source order. Pipe it
into anything and they do not:

```console
$ ./build/08-streams-and-exit < /dev/null | cat
this goes to stderr
exiting with EXIT_SUCCESS (0)
this goes to stdout
so does this -- printf IS fprintf(stdout, ...)
```

`stdout` switched to block buffering when it stopped being a terminal, so all
of it arrived at once at the end. `stderr` is never buffered. That is exactly
why Session 3 told you to debug with `fprintf(stderr, ...)` — it survives a
crash that loses everything still sitting in `stdout`'s buffer.
