# Session 15 — recursion and backtracking

```text
01-factorial-frames.c    each call's frame address, 64 bytes apart, growing down
02-bad-no-base-case.c    countdown by 2 from an odd number: never hits 0
02-good-base-case.c      n <= 0 instead of n == 0
03-fibonacci.c           naive vs memoised vs loop, with call counts
04-hanoi.c               the recursion that is clearer than any loop
05-permutations.c        backtracking: choose, explore, un-choose
06-n-queens.c            backtracking with pruning, vs brute force
```

`02-bad` is also built with AddressSanitizer as `02-bad-no-base-case-asan`.

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build
./run-all.sh
```

`02-bad` crashes on purpose. Pass an even number (`./build/02-bad-no-base-case 6`)
and it terminates normally.

## What to look for

### 01 — the frames are real

```console
  factorial(5)  frame at 0x7ffefb9ee4cc
  factorial(4)  frame at 0x7ffefb9ee48c
  factorial(3)  frame at 0x7ffefb9ee44c
```

Each call sits 64 bytes below the one that called it. Five copies of `n`
exist at the same time.

### 02 — no RecursionError in C

```console
depth 80000, n = -159991
Segmentation fault                    [exit code: 139]
```

Python would have stopped at depth 1000 with a traceback. C keeps going
until the 8 MB stack is gone. The ASan build at least names the function:

```text
SUMMARY: AddressSanitizer: stack-overflow ... 02-bad-no-base-case.c:16 in countdown
```

### 03 — exponential vs linear

```console
naive  fib(40) = 102334155      331160281 calls   0.133 s
memo   fib(40) = 102334155             79 calls   0.000 s
loop   fib(40) = 102334155             no         0.000 s
```

### 06 — pruning

```console
  N   solutions   placements tried   brute force N^N
  8          92              15720          16777216
 10         724             348150       10000000000
```

The solution counts (2, 10, 4, 40, 92, 352, 724 for N = 4 to 10) are the
well-known sequence. Check yours against them if you rewrite it.
