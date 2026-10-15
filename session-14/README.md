# Session 14 — linked lists

```text
01-node-basics.c                   three nodes, the traversal loop, their addresses
02-bad-push-by-value.c             push_front(Node *head) changes a copy -- list stays empty
02-good-push-pointer-to-pointer.c  push_front(Node **head)
03-bad-free-while-walking.c        for (...; n = n->next) free(n);
03-good-free-list.c                save next, then free
04-remove.c                        remove by walking Node ** links -- no head special case
05-reverse.c                       in place, three pointers
06-array-vs-list.c                 measured: walking, and inserting at the front
```

`02-bad` and `03-bad` are also built with AddressSanitizer as `<name>-asan`.

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build
./run-all.sh
```

`03-bad` aborts on purpose.

## What to look for

### 02 — C passes pointers by value too

```console
$ ./build/02-bad-push-by-value
head = (nil)
list:   <-- empty

$ ./build/02-bad-push-by-value-asan
Direct leak of 48 byte(s) in 3 object(s) allocated from:
    #1 ... in push_front .../02-bad-push-by-value.c:16
```

To change the caller's `Node *`, pass a `Node **` — `swap()` from Session 6,
one level up.

### 03 — the obvious free loop aborts

```console
warning: pointer 'n' used after 'free' [-Wuse-after-free]

$ ./build/03-bad-free-while-walking
free(): invalid pointer
Aborted
```

glibc stores its own bookkeeping inside a freed block, right where `next`
was. The loop then follows that value into another `free()`.

### 06 — the cache decides

```console
WALK 2000000 elements, summing each
  array            0.0006 s
  list, in order   0.0042 s     7.6x the array
  list, shuffled   0.2999 s   536.5x the array

INSERT AT FRONT, 50000 times
  array (memmove)  0.0688 s
  list  (push)     0.0014 s    48.7x faster
```

The shuffled list holds the same nodes as the in-order one, linked in a
different order. The slowdown is purely about where the next node sits in
memory.

The array walk takes well under a millisecond, so its ratios jump between
runs. Read them as orders of magnitude: about 10x and about 500x.
