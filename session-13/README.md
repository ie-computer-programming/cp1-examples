# Session 13 — enumerations and typedef

```text
01-enum-basics.c           auto-numbering, the _COUNT idiom, a name table
02-bad-enum-is-int.c       a C enum is an int: wrong enum, 42, arithmetic -- all compile
03-x-macro.c               one list generates the enum AND the names
04-bad-typedef-alias.c     typedef double Meters / Seconds -- mixing them compiles
04-good-struct-wrapper.c   a one-member struct is a real new type
05-function-pointer.c      typedef a comparator; qsort; a dispatch table
06-bad-typedef-pointer.c   typedef struct point *PointPtr hides where const goes
06-good-typedef-struct.c   typedef the struct, keep the * visible
```

## Build and run

```bash
conda activate programming
cmake -S . -B build && cmake --build build
./run-all.sh
```

## What to look for

### 02 — the compiler checks less than you think

```console
warning: implicit conversion from 'enum <anonymous>' to 'Color' [-Wenum-conversion]

a = green          <-- FRUIT_BANANA is 1, and so is COLOR_GREEN
b = ???            <-- Color b = 42 compiled without a word
n = 20
```

`-Wextra` catches the wrong-enum assignment. Nothing catches `42`.

### 04 — an alias is not a type

```console
position = 130.0 m   <-- 130 "meters", after adding 30 seconds
```

Uncomment the marked line in `04-good-struct-wrapper.c` and the same mistake
becomes:

```text
error: incompatible type for argument 2 of 'walk'
note: expected 'Meters' but argument is of type 'Seconds'
```

### 06 — `const PointPtr p` is not `const struct point *p`

```console
show: (999, 2)
main: (999, 2)   <-- changed by a function that promised not to
```

The `const` applied to the pointer, not to the point. Uncomment the marked
line in `06-good` to see the compiler refuse the same write:

```text
error: assignment of member 'x' in read-only object
```
