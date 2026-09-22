# Session 8 — structures and unions

```text
01-struct-basics.c          declaring, initialising, member access, copying
02-typedef-and-arrow.c      typedef; value.field vs pointer->field
03-bad-pass-by-value.c      a struct argument is COPIED -- the caller sees nothing
03-good-pass-by-pointer.c   Thing * to modify, const Thing * to read
04-padding-and-alignment.c  sizeof != sum of members; field order changes it
05-struct-on-heap.c         the create/destroy pair, and who owns what
06-bad-shallow-copy.c       b = a copies the pointer, not the string
06-good-deep-copy.c         write your own copy function; there is no deepcopy()
07-union-basics.c           one block of memory, several types
08-bad-union-untagged.c     a union does not remember which member is live
08-good-tagged-union.c      enum tag + union + switch = the safe pattern
09-aos-vs-soa.c             array of structs vs struct of arrays; cache lines
```

`06-bad-shallow-copy` is also built with AddressSanitizer as
`06-bad-shallow-copy-asan`. **Run the pair.**

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build

./run-all.sh          # every example, with its exit code
```

`06-bad-shallow-copy` aborts on purpose, so `run-all.sh` does not stop on
failure.

## What to look for

### 01 — a struct is not a dict

Fields are fixed at compile time. A typo is a compile error, not a
`KeyError`. `= {0}` zeroes everything. And there is **no `==`**: comparing
two structs member by member is the only option, because `memcmp` would also
compare the padding bytes (see 04).

### 03 — the Python surprise, in reverse

```console
$ ./build/03-bad-pass-by-value
before : 100.00
  inside deposit(): balance = 150.00
after  : 100.00   <-- unchanged
[exit code: 0]
```

The deposit worked. On a copy. Passing an object to a function in Python
hands over a reference; passing a struct in C hands over a duplicate. No
warning, no crash, wrong answer.

`const Account *` in `03-good` is the important half: it is the only part of
a C signature that tells the caller their data will not be modified — and the
compiler enforces it.

### 04 — sizeof is not the sum of the parts

```console
sum of members = 10
struct wasteful: sizeof = 24
struct tidy:     sizeof = 16
same fields, 8 bytes saved by declaring the big ones first
```

`char, double, char` costs 24 bytes; `double, char, char` costs 16. The
compiler aligns each member to its own size and pads the gaps. Declaring
members **largest-first** is a free 33 % here.

### 06 — the copy that is not a copy

```console
$ ./build/06-bad-shallow-copy
a.name = 0x5b2c0cb32010  b.name = 0x5b2c0cb32010
a.name = Xda   <-- changed through b
free(): double free detected in tcache 2
[exit code: 134]
```

Two structs, one string. ASan names both frees and the original `malloc`:

```console
$ ./build/06-bad-shallow-copy-asan
ERROR: AddressSanitizer: attempting double-free ...
    #1 ... in main .../06-bad-shallow-copy.c:46
freed by thread T0 here:
    #1 ... in main .../06-bad-shallow-copy.c:45
```

**If a struct contains an owned pointer, you must write the copy function.**

### 07 / 08 — a union is a reinterpretation

```console
sizeof(union value) = 4   (largest member)
sizeof(struct pair) = 8   (all members)

wrote  as_int   = 1
bytes           = 01 00 00 00
read   as_float = 1.4013e-45
```

Same four bytes, two meanings. Useful for inspecting representations, and
the foundation of the **tagged union**: an `enum` saying which member is
live, stored next to the union in a struct. `08-bad` shows what you get
without the tag (`1374389535` — a perfectly valid, perfectly meaningless
`int`).

In `08-good`, `number_print` has **no `default:` case** on purpose. Add a
value to the enum and forget to handle it, and `-Wswitch` says so at compile
time.

### 09 — layout is a performance decision

```console
sum of x, array of structs : 198000000 in 0.0103 s
sum of x, struct of arrays : 198000000 in 0.0045 s
same answer, 2.3x
```

A cache line is 64 bytes. Summing one `float` field out of a 16-byte struct
wastes 48 of every 64 bytes fetched. Split the fields into parallel arrays
and the same loop runs ~2× faster with identical arithmetic. Your numbers
will differ — the ratio is the point, not the value.

This is the whole idea behind **data-oriented design**: organise data around
how it is accessed, not around how it is named.
