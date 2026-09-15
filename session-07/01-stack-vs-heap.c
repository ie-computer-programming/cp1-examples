/* Two very different places a pointer can point.
 *
 * A local variable lives on the STACK: the compiler decided its size and
 * lifetime when it compiled this function.
 *
 * A malloc'd block lives on the HEAP: you decide its size while the program
 * runs, and it stays alive until you say otherwise.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int on_stack = 42;

    int *on_heap = malloc(sizeof *on_heap);
    if (on_heap == NULL) {          /* malloc CAN fail -- Session 3 */
        perror("malloc");
        return 1;
    }
    *on_heap = 42;

    printf("same value, different neighbourhoods:\n");
    printf("  stack variable at %p\n", (void *)&on_stack);
    printf("  heap block     at %p\n", (void *)on_heap);

    /* The size below is a RUNTIME value. You cannot write int a[n] and have
     * it outlive this function; malloc has no such restriction. */
    size_t n = 5;
    int *a = malloc(n * sizeof *a);
    if (a == NULL) {
        perror("malloc");
        free(on_heap);
        return 1;
    }
    for (size_t i = 0; i < n; i++) {
        a[i] = (int)(i * i);
    }
    printf("  %zu ints on the heap:", n);
    for (size_t i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    free(a);
    free(on_heap);
    return 0;
}
