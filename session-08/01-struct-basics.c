/* 01 -- declaring a struct, initialising it, reading its members.
 *
 * A struct is a fixed set of named fields, laid out in one block of memory.
 * Python's closest relatives are a dataclass (fixed fields) and a tuple
 * (fixed layout). It is NOT a dict: you cannot add a field at runtime, and
 * a typo in a field name is a compile error rather than a KeyError.
 */
#include <stdio.h>

struct point {
    int x;
    int y;
};

int main(void)
{
    /* Positional, in declaration order. */
    struct point a = {3, 4};

    /* Designated initialisers (C99). Order-independent, and anything you
     * leave out is zero-initialised -- so this is also how you get a
     * guaranteed-blank struct: struct point z = {0}; */
    struct point b = {.y = 10, .x = 7};
    struct point origin = {0};

    printf("a = (%d, %d)\n", a.x, a.y);
    printf("b = (%d, %d)\n", b.x, b.y);
    printf("origin = (%d, %d)\n", origin.x, origin.y);

    /* Members are ordinary lvalues. */
    a.x += 1;
    printf("a after a.x += 1 = (%d, %d)\n", a.x, a.y);

    /* Whole-struct assignment copies every member. Two independent objects. */
    struct point copy = a;
    copy.y = 99;
    printf("a    = (%d, %d)\n", a.x, a.y);
    printf("copy = (%d, %d)\n", copy.x, copy.y);

    /* There is no == for structs. This does not compile:
     *     if (a == copy) { ... }
     * Compare field by field, or write an equals() function. */
    printf("a == copy ? %s\n",
           (a.x == copy.x && a.y == copy.y) ? "yes" : "no");

    return 0;
}
