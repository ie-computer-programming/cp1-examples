/* Value, address, and the value reached through a pointer.
 * Run this first -- everything else today builds on these three lines. */
#include <stdio.h>

int main(void)
{
    int x = 42;
    int *p = &x;        /* p holds the address of x */

    printf("x   = %d\n", x);      /* the value            */
    printf("&x  = %p\n", (void *)&x);   /* where x lives  */
    printf("p   = %p\n", (void *)p);    /* p holds that   */
    printf("*p  = %d\n", *p);     /* follow p, read there */
    printf("&p  = %p\n", (void *)&p);   /* p has its OWN address */

    *p = 99;                      /* follow p, write there */
    printf("after *p = 99, x = %d\n", x);

    return 0;
}
