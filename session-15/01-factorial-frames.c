/* 01 -- recursion, and the stack frames that make it work.
 *
 * Every call gets its own frame on the stack (Session 7): its own copy of
 * `n`, its own locals, its own return address. Recursion is nothing more
 * than a function whose frames happen to belong to itself.
 *
 * Watch the addresses: each deeper call lives LOWER in memory, because the
 * stack grows downwards on x86 and ARM.
 */
#include <stdio.h>

static unsigned long long factorial(unsigned n)
{
    int marker;     /* exists only so we can print where this frame lives */
    printf("  factorial(%u)  frame at %p\n", n, (void *)&marker);

    if (n <= 1) {
        return 1;                       /* base case: stops the descent */
    }
    return n * factorial(n - 1);        /* recursive case: a SMALLER problem */
}

int main(void)
{
    unsigned long long r = factorial(5);
    printf("5! = %llu\n", r);
    return 0;
}
