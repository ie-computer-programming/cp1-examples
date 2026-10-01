/* 04-good -- if it behaves like a function, write a function.
 *
 * `static inline` gives you what the macro was for -- no call overhead at any
 * optimisation level above -O0 -- and everything the macro could not:
 *
 *   - arguments evaluated exactly ONCE
 *   - real types, so the compiler checks them
 *   - a name the debugger can show you and a line you can break on
 *   - normal precedence, no parentheses discipline required
 *
 * The one thing a macro still has over it: a macro works for ANY type.
 * That is why MAX survives in real code -- and why it is written in capitals,
 * to warn you that it is not a function.
 */
#include <stdio.h>

static inline int max_int(int a, int b)
{
    return a > b ? a : b;
}

int main(void)
{
    int i = 5, j = 3;

    printf("before : i = %d, j = %d\n", i, j);

    int m = max_int(i++, j++);

    printf("max_int(i++, j++) = %d\n", m);
    printf("after  : i = %d, j = %d   <-- each incremented once\n", i, j);

    return 0;
}
