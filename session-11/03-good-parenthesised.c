/* 03-good -- parenthesise every parameter, AND the whole body.
 *
 *     #define SQUARE(x) ((x) * (x))
 *                       ^         ^   the outer pair protects the macro from
 *                        ^     ^      whatever surrounds the call;
 *                                     the inner pairs protect each argument.
 *
 * Both are needed. ((x) * (x)) without the outer parens breaks
 * `100 / SQUARE(5)`; (x * x) without the inner ones breaks `SQUARE(2 + 3)`.
 */
#include <stdio.h>

#define SQUARE(x)  ((x) * (x))
#define HALF(x)    ((x) / 2)

int main(void)
{
    printf("SQUARE(5)       = %d\n", SQUARE(5));
    printf("SQUARE(2 + 3)   = %d\n", SQUARE(2 + 3));
    printf("100 / SQUARE(5) = %d\n", 100 / SQUARE(5));
    printf("HALF(4 + 4)     = %d\n", HALF(4 + 4));

    return 0;
}
