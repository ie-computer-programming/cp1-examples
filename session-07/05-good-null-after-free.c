/* GOOD: free, then immediately NULL.
 *
 * Setting the pointer to NULL after freeing turns two invisible bugs into
 * one obvious one:
 *   - use-after-free  becomes an immediate, reproducible crash at the guard
 *   - double free     becomes free(NULL), which is legal and does nothing
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
    p = NULL;            /* the whole trick */

    if (p != NULL) {
        printf("after free:  %d\n", *p);
    } else {
        printf("after free:  pointer is NULL, nothing to read\n");
    }

    free(p);             /* free(NULL) is a no-op -- no double free */

    printf("clean exit\n");
    return 0;
}
