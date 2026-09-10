/* Reading past the end of an array.
 * Plain build: usually prints garbage and carries on, as if nothing is wrong.
 * With -fsanitize=address: the exact file and line.
 *
 *   cc -g -fsanitize=address -o demo 04-out-of-bounds.c && ./demo
 */
#include <stdio.h>

int main(void)
{
    int a[5] = {10, 20, 30, 40, 50};

    printf("a[4]  = %d\n", a[4]);    /* fine: the last element */
    printf("a[10] = %d\n", a[10]);   /* NOT fine: past the end */

    return 0;
}
