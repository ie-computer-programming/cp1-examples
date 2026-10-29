# Session 18 — process management and IPC

```text
01-fork-basics.c               fork() returns twice; the child gets a COPY
02-bad-zombie.c                a child nobody waits for: ps shows STAT Z
02-good-wait.c                 waitpid + WIFEXITED / WIFSIGNALED (134 = 128 + 6)
03-bad-exec-falls-through.c    no _exit after exec: two processes run on
03-good-exec.c                 fork + exec + wait: how a shell runs a command
04-pipe.c                      a one-way channel between parent and child
05-bad-pipe-hang.c             a forgotten close(): the reader never sees EOF
06-shared-memory.c             mmap(MAP_SHARED): fast, and racy
07-message-queue.c             typed messages with boundaries (System V)
08-socketpair.c                a two-way channel; the same send/recv as Session 19
```

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build
./run-all.sh
```

This session needs `fork()` and POSIX IPC: **Linux, macOS or WSL**. It does not
build on native Windows.

`05-bad-pipe-hang` hangs on purpose; an alarm ends it after 2 seconds.

> **macOS:** I could only test on Linux. POSIX message queues (`mq_open`) do not
> exist on macOS, so `07` uses System V queues, which do. `06` falls back to
> `MAP_ANON` if `MAP_ANONYMOUS` is missing; that fallback is untested.

## What to look for

### 01 — flush before you fork

```console
$ ./build/01-fork-basics | cat        # with the fflush removed
before fork: pid 129960, x = 100
child : pid 129962, parent 129960, x = 101
before fork: pid 129960, x = 100      <-- printed a second time
parent: pid 129960, child was 129962, x = 100
```

Piped, stdout is block-buffered (Session 9). The text was still in the buffer
when `fork()` copied the process, so both processes flushed it.

### 02 — zombies, and how a child really ended

```console
$ ./build/02-bad-zombie
 130051 Z+   02-bad-zombie

$ ./build/02-good-wait
child 130079 exited normally with code 7
child 130080 was killed by signal 6  (a shell would report 134)
```

### 05 — close every end you don't use

```console
$ ./build/05-bad-pipe-hang
child got 5 bytes
HUNG: the child is still waiting for data -- killed by alarm
```

`read()` returns 0 only when every copy of the write end is closed, including
the child's own inherited one.

### 06 — shared memory has no protection

```console
ordinary variable : 0
shared memory     : 440147   expected 1000000   lost 559853 increments
shared memory     : 750000   expected 1000000   lost 250000 increments
```

Three runs lost between 25% and 56% of the increments. The amount changes every
run. This example is built at `-O0` on purpose, for the same reason as Session
17's counter.

### 07 — kernel objects outlive the program

```console
$ ipcs -q        # lists message queues; nothing should be left after a clean run
```

If a run crashes before `msgctl(IPC_RMID)`, remove the leftover with `ipcrm -q <id>`.
