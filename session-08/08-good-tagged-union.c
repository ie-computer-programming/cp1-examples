/* 08-good -- the tagged union: an enum that says which member is live.
 *
 * A struct containing (a) a tag and (b) a union. This is what Python's
 * `int | float | str` type hint compiles down to conceptually, and what Rust
 * calls an enum. In C you have to maintain the tag by hand -- but once you
 * do, `switch` on it and the compiler will warn about the case you forgot.
 *
 * Use it whenever a value is "one of these things": a token, a JSON node, a
 * network message, a result-or-error.
 */
#include <stdio.h>

typedef enum {
    NUMBER_INT,
    NUMBER_DOUBLE,
    NUMBER_NONE
} NumberKind;

typedef struct {
    NumberKind kind;
    union {
        int    i;
        double d;
    } as;          /* anonymous-ish: reached as n.as.i / n.as.d */
} Number;

static Number number_from_int(int i)
{
    return (Number){.kind = NUMBER_INT, .as.i = i};   /* compound literal */
}

static Number number_from_double(double d)
{
    return (Number){.kind = NUMBER_DOUBLE, .as.d = d};
}

static void number_print(const Number *n)
{
    /* No `default:` on purpose. Adding a kind to the enum without handling it
     * here is then caught by -Wswitch, at compile time. */
    switch (n->kind) {
    case NUMBER_INT:    printf("  int    %d\n", n->as.i);  break;
    case NUMBER_DOUBLE: printf("  double %g\n", n->as.d);  break;
    case NUMBER_NONE:   printf("  none\n");                break;
    }
}

int main(void)
{
    Number values[] = {
        number_from_int(42),
        number_from_double(3.14),
        {.kind = NUMBER_NONE},
    };

    printf("sizeof(Number) = %zu (tag + largest member + padding)\n",
           sizeof(Number));

    for (size_t i = 0; i < sizeof values / sizeof values[0]; i++) {
        number_print(&values[i]);
    }

    return 0;
}
