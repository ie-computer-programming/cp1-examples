# Session 16 — thread concepts and lifecycle

```text
01-hello-threads.c          create 4 threads, join them; the order changes every run
02-bad-no-join.c            main returns without joining -- the workers' output is lost
02-good-join.c              join every thread
03-bad-loop-variable.c      pthread_create(..., &i) -- every thread shares one address
03-good-per-thread-args.c   one struct per thread, results read after join
04-parallel-sum.c           speed-up measured: compute-bound vs memory-bound
05-race-preview.c           4 threads, 1 shared counter -- fixed in Session 17
```

`03-bad` and `05` are also built with ThreadSanitizer as `<name>-tsan`.

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build
```

**Run things more than once.** With threads, one run proves nothing.

> **macOS:** the `-tsan` twins were tested on Linux only. If they fail to
> build or link on your Mac, delete the `foreach(... -tsan ...)` block from
> `CMakeLists.txt`; everything else is plain POSIX and works unchanged.

## What to look for

### 01 — the OS picks the order

```console
hello from thread 0   2   1   3      (run 1)
hello from thread 0   3   1   2      (run 2)
hello from thread 0   1   2   3      (run 3)
```

### 02 — returning from main ends every thread

```console
$ ./build/02-bad-no-join
main: returning   <-- no pthread_join
```

None of the four "worker N done" lines appeared, in three runs out of three.

### 03 — `&i` is one address, shared by everyone

```console
ids seen = 1 2 4 5 6 6
ids seen = 5 6 6 6 6 6        <-- 6: the value i has AFTER the loop ends
```

### 04 — speed-up depends on what limits the work

```console
PRIMES -- compute-bound              SUM -- memory-bound
threads   speed-up                   threads   speed-up
      2     1.88x                          2     1.85x
      8     5.55x                          8     2.75x
     16     7.68x                         16     3.02x
```

Counting primes is arithmetic, so more cores help. Summing an array is mostly
waiting for RAM, so they barely do. Your numbers will differ; the shape will not.

The CPU-time column stays roughly constant as threads are added: `clock()`
adds up CPU time across threads. Use `clock_gettime(CLOCK_MONOTONIC, ...)`.

Set `CHUNK` to 1 and run it again: thread 0 then gets only even numbers.

### 05 — the cliffhanger

```console
counter = 1356190   expected 4000000   lost 2643810 increments
counter = 1696734   expected 4000000   lost 2303266 increments
```

```text
WARNING: ThreadSanitizer: data race
  Location is global 'counter' of size 8
```
