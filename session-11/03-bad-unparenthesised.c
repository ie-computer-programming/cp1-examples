/* 03-bad -- a macro is text, so precedence is NOT what you wrote.
 *
 *     #define SQUARE(x) x * x
 *     SQUARE(2 + 3)   becomes   2 + 3 * 2 + 3   ==  11, not 25
 *
 * You wrote an expression. The preprocessor pasted tokens. The compiler then
 * applied its own precedence rules to the result, which is not the grouping
 * you had in your head.
 *
 * A function would not do this: its arguments are evaluated before the call.
 * A macro has no arguments, only text.
 */
#include <stdio.h>

#define SQUARE(x)  x * x
#define HALF(x)    x / 2

int main(void)
{
    printf("SQUARE(5)      = %d   (fine, by luck)\n", SQUARE(5));
    printf("SQUARE(2 + 3)  = %d   <-- expected 25\n", SQUARE(2 + 3));
    printf("100 / SQUARE(5) = %d  <-- expected 4\n", 100 / SQUARE(5));
    printf("HALF(4 + 4)    = %d   <-- expected 4\n", HALF(4 + 4));

    printf("exit code will be 0\n");
    return 0;
}
