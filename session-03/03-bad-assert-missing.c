/* BAD: no assertion, so a broken call silently produces a nonsense answer.
   Nothing crashes. Nothing warns. The number is just wrong. */
#include <stddef.h>
#include <stdio.h>

static double average(const int *values, size_t count)
{
    long sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += values[i];
    }
    return (double)sum / (double)count;   /* count == 0 -> 0.0 / 0.0 -> nan */
}

int main(void)
{
    int data[] = {1, 2, 3};

    printf("average of 3: %f\n", average(data, 3));
    printf("average of 0: %f\n", average(data, 0));

    return 0;
}
