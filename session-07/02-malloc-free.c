/* The whole lifecycle, in one file: ask, check, use, give back.
 *
 *   malloc(n)      n bytes, contents are GARBAGE
 *   calloc(k, n)   k * n bytes, contents are ZERO
 *   free(p)        give the block back; p is now meaningless
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t n = 4;

    /* sizeof *p, not sizeof(int): if you change the type of p, this line is
     * still correct. This is the idiom to internalise. */
    int *p = malloc(n * sizeof *p);
    if (p == NULL) {
        perror("malloc");
        return 1;
    }

    /* malloc does NOT initialise. Reading p[0] right now is undefined. */
    for (size_t i = 0; i < n; i++) {
        p[i] = (int)i + 1;
    }
    printf("malloc + fill :");
    for (size_t i = 0; i < n; i++) {
        printf(" %d", p[i]);
    }
    printf("\n");

    int *z = calloc(n, sizeof *z);   /* zeroed for you */
    if (z == NULL) {
        perror("calloc");
        free(p);
        return 1;
    }
    printf("calloc        :");
    for (size_t i = 0; i < n; i++) {
        printf(" %d", z[i]);
    }
    printf("\n");

    free(z);
    free(p);

    /* free(NULL) is explicitly legal and does nothing -- which is why the
     * cleanup path of a half-built object can free everything blindly. */
    free(NULL);

    printf("freed cleanly\n");
    return 0;
}
