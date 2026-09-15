/* BAD: use after free.
 *
 * free() does not erase the block and does not change your pointer. The
 * pointer still holds the same address -- it just no longer owns what is
 * there. The allocator may already have handed those bytes to someone else.
 *
 * This is one of the most exploited bug classes in real software.
 *
 * Watch it get caught:
 *     ./build/05-bad-use-after-free-asan
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p = malloc(sizeof *p);
    if (p == NULL) {
        perror("malloc");
        return 1;
    }

    *p = 42;
    printf("before free: %d\n", *p);

    free(p);
    /* p is now a DANGLING pointer: same address, no longer ours. */

    printf("after free:  %d\n", *p);   /* <-- reading freed memory */
    *p = 99;                           /* <-- writing freed memory, worse */

    free(p);                           /* <-- double free, worse again */

    printf("still here\n");
    return 0;
}
