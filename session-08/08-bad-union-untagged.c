/* 08-bad -- a bare union does not remember which member is live.
 *
 * The union stores the bytes. It does not store WHICH member you wrote.
 * Nothing in the type system, the compiler or the runtime will stop you from
 * reading the wrong one. You get a number. It is meaningless. It is not an
 * error.
 */
#include <stdio.h>

union number {
    int    i;
    double d;
};

/* The function has no way to know what the caller put in there. */
static void print_as_int(union number n)
{
    printf("  as int    : %d\n", n.i);
}

int main(void)
{
    union number n;

    n.i = 42;
    printf("wrote n.i = 42\n");
    print_as_int(n);

    n.d = 3.14;
    printf("wrote n.d = 3.14\n");
    print_as_int(n);        /* garbage -- but a perfectly valid int */

    printf("exit code will be 0\n");
    return 0;
}
