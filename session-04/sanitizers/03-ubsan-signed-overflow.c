/* UBSan: signed integer overflow.
 * This is exactly the HW1 `cp1_add_checked` trap -- and the reason the task
 * says to DETECT overflow rather than compute the sum and inspect it.
 * Signed overflow is undefined behaviour, so by then it is already too late.
 *
 *   cc -g -fsanitize=undefined -o demo 03-ubsan-signed-overflow.c && ./demo
 */
#include <limits.h>
#include <stdio.h>

static int add_unchecked(int a, int b)
{
    return a + b;               /* undefined when it overflows */
}

int main(void)
{
    printf("INT_MAX     = %d\n", INT_MAX);
    printf("INT_MAX + 1 = %d\n", add_unchecked(INT_MAX, 1));
    return 0;
}
