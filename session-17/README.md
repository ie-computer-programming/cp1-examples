# Session 17 — advanced thread management

```text
01-fixing-the-counter.c    Session 16's race, fixed four ways and timed
02-bad-check-then-act.c    every line locked, balance still goes to -700
02-good-whole-section.c    one lock around the whole check-and-act
03-bad-deadlock.c          two locks, opposite order: the program just stops
03-good-lock-order.c       one global lock order
04-semaphore-pool.c        8 workers, 3 connections: at most 3 at once
05-barrier.c               nobody starts step 2 until everyone finishes step 1
06-openmp.c                Session 16's parallel work in one #pragma
include/barrier_compat.h   pthread barriers for macOS, which lacks them
```

`-tsan` twins: `01-fixing-the-counter-tsan`, `03-bad-deadlock-tsan`.

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build
```

`03-bad-deadlock` hangs on purpose; an alarm kills it after 2 seconds.

### macOS

- **Semaphores:** `sem_init` is not implemented on macOS, so `04` uses
  `sem_open` (a *named* semaphore), which works on Linux and macOS.
- **Barriers:** macOS has no `pthread_barrier_t`. `include/barrier_compat.h`
  supplies one, only on Apple. It was tested by forcing it on under Linux
  (`-DCP1_FORCE_BARRIER_SHIM`), including under ThreadSanitizer.
- **OpenMP:** Apple's clang ships without it. If CMake prints
  `OpenMP not found -- skipping 06-openmp`, the rest still builds. To enable it,
  `conda install -c conda-forge llvm-openmp` in the `programming` environment.
- **ThreadSanitizer** was tested on Linux only.

## What to look for

### 01 — correctness has a price; sharing less avoids it

```console
method           result     verdict   time
racy               1225334  WRONG      0.0096 s
mutex per ++       4000000  correct    0.1702 s
atomic             4000000  correct    0.0675 s
local, lock once   4000000  correct    0.0008 s
```

This example is built at `-O0` on purpose. At `-O2`, gcc turns the racy loop
into a single `counter += 1000000`, so the racy row prints **correct**. It is
still a data race, and ThreadSanitizer still reports it:

```text
SUMMARY: ThreadSanitizer: data race 01-fixing-the-counter.c:26 in racy
racy               4000000  correct    0.0435 s
```

### 02 — locking each line is not enough

```console
8 withdrawals of 100 from 100 -> balance = -700
```

The lock is released between the balance check and the subtraction, so all
eight threads pass the check before any of them subtracts.

### 03 — TSan finds the deadlock on a run that works

The `-tsan` build runs the two threads one after the other, so it never hangs.
Both lock orders are still used:

```text
WARNING: ThreadSanitizer: lock-order-inversion (potential deadlock)
transfer: done
audit: done
```

### 06 — OpenMP

```console
1 thread(s): 216816 primes in 0.7942 s
8 thread(s): 216816 primes in 0.0928 s
```
