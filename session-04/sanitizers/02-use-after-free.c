/* ASan: using memory after it has been freed.
 * Without a sanitizer this often "works" -- the freed memory usually still
 * holds the old bytes, right up until something else reuses it.
 *
 *   cc -g -fsanitize=address -o demo 02-use-after-free.c && ./demo
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *values = malloc(4 * sizeof *values);
    if (values == NULL) {
        return 1;
    }

    values[0] = 42;
    free(values);

    printf("%d\n", values[0]);   /* the memory is no longer ours */
    return 0;
}
