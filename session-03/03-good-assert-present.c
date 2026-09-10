/* GOOD: the contract is stated, so a broken call stops here instead of
   returning a nonsense number that spreads through the rest of the program. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static double average(const int *values, size_t count)
{
    assert(values != NULL);
    assert(count > 0);

    long sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += values[i];
    }
    return (double)sum / (double)count;
}

int main(void)
{
    int data[] = {1, 2, 3};

    printf("average of 3: %f\n", average(data, 3));
    printf("average of 0: ");
    fflush(stdout);                       /* so the text appears before abort() */
    printf("%f\n", average(data, 0));     /* assertion fires here */

    return 0;
}
