/* The same traversal written two ways, plus the proof that an array's length
 * does not survive being passed to a function. */
#include <stdio.h>
#include <stddef.h>

static void print_by_index(const int *a, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    putchar('\n');
}

static void print_by_pointer(const int *a, size_t n)
{
    for (const int *p = a; p < a + n; p++) {
        printf("%d ", *p);
    }
    putchar('\n');
}

static void show_lost_length(const int a[])
{
    /* This line warns: "'sizeof' on array function parameter will return size
       of 'const int *'". That warning IS the lesson -- the compiler is telling
       you the length did not survive the call. */
    printf("inside the function, sizeof(a) = %zu (a pointer)\n", sizeof(a));
}

int main(void)
{
    int a[5] = {10, 20, 30, 40, 50};
    const size_t n = sizeof(a) / sizeof(a[0]);

    printf("in main, sizeof(a) = %zu bytes, so n = %zu\n", sizeof(a), n);
    show_lost_length(a);

    print_by_index(a, n);
    print_by_pointer(a, n);

    printf("a[2]=%d  *(a+2)=%d  -- the same thing\n", a[2], *(a + 2));

    return 0;
}
