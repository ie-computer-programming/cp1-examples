/* GOOD: every malloc has exactly one matching free.
 *
 * The habit worth building: write the free() the moment you write the
 * malloc(), before you write the code in between.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    long total = 0;

    for (int i = 0; i < 100; i++) {
        int *block = malloc(64 * sizeof *block);
        if (block == NULL) {
            perror("malloc");
            return 1;
        }
        block[0] = i;
        total += block[0];
        free(block);
    }

    printf("total = %ld\n", total);
    printf("no leak\n");
    return 0;
}
