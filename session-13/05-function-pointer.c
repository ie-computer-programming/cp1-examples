/* 05 -- where typedef earns its keep: naming a function-pointer type.
 *
 * A function has an address, like any variable, so you can store it and
 * call it later. Python does this every time you write sorted(xs, key=f).
 *
 * The raw C spelling is famously unreadable:
 *     int (*cmp)(const void *, const void *)
 * typedef gives it a name, and then it reads like any other type.
 */
#include <stdio.h>
#include <stdlib.h>

typedef int (*Comparator)(const void *a, const void *b);

static int ascending(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);        /* -1, 0 or 1 -- and cannot overflow */
}

static int descending(const void *a, const void *b)
{
    return ascending(b, a);
}

static void sort_and_print(const char *label, int *v, size_t n, Comparator cmp)
{
    qsort(v, n, sizeof *v, cmp);     /* qsort calls cmp, not the other way round */
    printf("%-11s", label);
    for (size_t i = 0; i < n; i++) {
        printf(" %d", v[i]);
    }
    printf("\n");
}

int main(void)
{
    int v[] = {42, -7, 19, 0, 3};
    size_t n = sizeof v / sizeof v[0];

    sort_and_print("ascending:", v, n, ascending);
    sort_and_print("descending:", v, n, descending);

    /* A table of functions, chosen at runtime -- a tiny dispatch table. */
    Comparator choices[] = { ascending, descending };
    sort_and_print("choices[0]:", v, n, choices[0]);
    return 0;
}
