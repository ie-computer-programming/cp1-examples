/* Pass by value vs. pass by address.
 * swap_broken gets copies; swap gets addresses and can reach the originals. */
#include <stdio.h>

static void swap_broken(int a, int b)
{
    int t = a;
    a = b;
    b = t;
}

static void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int main(void)
{
    int x = 1, y = 2;

    swap_broken(x, y);
    printf("after swap_broken: x=%d y=%d\n", x, y);   /* unchanged */

    swap(&x, &y);
    printf("after swap:        x=%d y=%d\n", x, y);   /* swapped   */

    return 0;
}
