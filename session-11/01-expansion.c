/* 01 -- the preprocessor is a TEXT pass that runs before the compiler.
 *
 * It does not know C. It does not know types, scopes or expressions. It
 * pastes text, and then hands the result to the compiler.
 *
 * See it for yourself:
 *     gcc -E 01-expansion.c | tail -20
 *
 * That is the `cpp` stage from the Session 1 pipeline, stopped and printed.
 */
#include <stdio.h>

#define COURSE       "CP1"
#define YEAR         2026
#define GREETING     "Hello from " COURSE      /* string literals concatenate */
#define TWICE(x)     ((x) + (x))

int main(void)
{
    /* By the time the compiler sees this line, every name below has already
     * been replaced by its text. The compiler never hears the word COURSE. */
    printf("%s, %d\n", GREETING, YEAR);
    printf("TWICE(21) = %d\n", TWICE(21));

    /* A macro is not a variable: there is nothing to inspect at runtime and
     * nothing for the debugger to show you. It is gone before compilation. */
    return 0;
}
