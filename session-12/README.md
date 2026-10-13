# Session 12 — bit manipulation

```text
include/bits.h            print_bits(): show a value in binary
01-operators.c            & | ^ ~ << >> on a uint8_t, in binary
02-masks.c                set / clear / toggle / test one bit
03-bad-precedence.c       flags & READY == 0  is  flags & (READY == 0)
03-good-precedence.c      (flags & READY) == 0
04-bad-shift.c            1 << 31 and 1u << 32 -- both undefined
04-good-shift.c           unsigned, fixed-width, checked shift amount
05-twos-complement.c      ~x + 1, one zero, INT_MIN has no positive twin
06-enum-flags.c           permission bits: rwx, chmod 754
07-morton.c               interleaving bits: the Z-order curve
08-bad-rsqrt.c            Quake III's fast inverse square root, verbatim
08-good-rsqrt.c           the same algorithm with memcpy -- and its error
```

Twins built with a sanitizer — **run the pair**:

```text
04-bad-shift-ubsan        -fsanitize=undefined
08-bad-rsqrt-asan         -fsanitize=address
```

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build

./run-all.sh
```

Nothing here crashes in a plain build. Every bug in this folder produces a
plausible answer.

## What to look for

### 03 — one missing pair of parentheses

```console
$ ./build/03-bad-precedence
ready -- starting   <-- wrong, the bit is clear
```

`==` binds tighter than `&`, so the test is `flags & (READY == 0)`, which is
`flags & 0`, which is always false. gcc says so:

```text
warning: suggest parentheses around comparison in operand of '&' [-Wparentheses]
```

### 04 — `1u << 32` is `1`

```console
$ ./build/04-bad-shift
n = 31   1u << n = 0x80000000   1 << n = -2147483648
n = 32   1u << n = 0x00000001   1 << n = 1
```

Shifting by the full width is undefined. x86 happens to use only the low five
bits of the shift count, so `<< 32` behaves like `<< 0`. The UBSan build names
both problems:

```text
04-bad-shift.c:19:27: runtime error: left shift of 1 by 31 places cannot be represented in type 'int'
04-bad-shift.c:18:28: runtime error: shift exponent 32 is too large for 32-bit type 'unsigned int'
```

There is no compile warning, because the loop bound comes from `argv`.

### 05 — two's complement

```console
5              5   0000_0101
~5            -6   1111_1010
~5 + 1        -5   1111_1011
-128        -128   1000_0000
```

Negation is "flip every bit, then add one". There is one more negative value
than positive, which is why `abs(INT_MIN)` is undefined.

### 07 — the Z

```console
   0   1   4   5
   2   3   6   7
   8   9  12  13
  10  11  14  15
```

Follow the numbers in order: a Z, made of smaller Zs.

### 08 — brilliant maths, undefined code

The plain build of the 1999 original prints the same answers as the fixed
version. At `-O2` gcc is far less happy:

```text
warning: dereferencing type-punned pointer will break strict-aliasing rules [-Wstrict-aliasing]
warning: 'y' is used uninitialized [-Wuninitialized]
warning: array subscript 'long int[0]' is partly outside array bounds of 'float[1]' [-Warray-bounds=]
```

`'y' is used uninitialized` is reported on the line right after `y = number`.
Under strict aliasing the optimiser may assume a `long *` never reads a
`float`, so as far as it is concerned that store never reaches the read.
And because `long` is 8 bytes on 64-bit systems, ASan catches the read
running 4 bytes past `y`:

```text
ERROR: AddressSanitizer: stack-buffer-overflow ... READ of size 8
    [48, 52) 'y' (line 19) <== Memory access at offset 48 partially overflows this variable
```

`08-good-rsqrt` does the same thing with `memcpy` and a `uint32_t`. Worst
relative error over `[0.001, 1000)`: **0.1752 %**.
