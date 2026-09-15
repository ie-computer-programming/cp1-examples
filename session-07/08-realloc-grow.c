/* realloc: the growable array -- what Python's list does for you.
 *
 * append() in Python hides exactly this: keep a capacity, and when it runs
 * out, ask for a bigger block and copy. Doubling means the copying cost
 * averages out to nothing over many appends.
 *
 * The one rule: NEVER write  a = realloc(a, ...)  directly. If realloc fails
 * it returns NULL and the ORIGINAL block is still alive -- assigning NULL
 * over your only pointer to it leaks it and loses your data.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t count = 0;
    size_t capacity = 2;

    int *a = malloc(capacity * sizeof *a);
    if (a == NULL) {
        perror("malloc");
        return 1;
    }

    for (int value = 1; value <= 10; value++) {
        if (count == capacity) {
            size_t new_capacity = capacity * 2;

            int *tmp = realloc(a, new_capacity * sizeof *a);
            if (tmp == NULL) {          /* a is still valid here */
                perror("realloc");
                free(a);
                return 1;
            }
            a = tmp;                    /* only now is the old pointer stale */
            capacity = new_capacity;
            printf("grew to capacity %zu\n", capacity);
        }
        a[count++] = value;
    }

    printf("%zu values:", count);
    for (size_t i = 0; i < count; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}
