#include "stats.h"

#include <assert.h>
#include <limits.h>

int stats_sum(const int *values, size_t n, int *out)
{
    assert(values != NULL);
    assert(out != NULL);

    int sum = 0;
    for (size_t i = 0; i < n; i++) {
        const int v = values[i];
        /* Detect overflow BEFORE causing it: signed overflow is undefined,
           so "add and then check the result" is already too late. */
        if ((v > 0 && sum > INT_MAX - v) || (v < 0 && sum < INT_MIN - v)) {
            return -1;
        }
        sum += v;
    }

    *out = sum;
    return 0;
}

int stats_mean(const int *values, size_t n, double *out)
{
    assert(values != NULL);
    assert(out != NULL);

    if (n == 0) {
        return -1;
    }

    /* Accumulated as double so a long run of large values cannot overflow. */
    double total = 0.0;
    for (size_t i = 0; i < n; i++) {
        total += (double)values[i];
    }

    *out = total / (double)n;
    return 0;
}

int stats_max(const int *values, size_t n, int *out)
{
    assert(values != NULL);
    assert(out != NULL);

    if (n == 0) {
        return -1;
    }

    int best = values[0];
    for (size_t i = 1; i < n; i++) {
        if (values[i] > best) {
            best = values[i];
        }
    }

    *out = best;
    return 0;
}
