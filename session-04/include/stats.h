/* stats.h — the library under test in Session 4.
 *
 * Note: this header has NO extern "C" guards, exactly like your HW1 `cp1.h`.
 * That is deliberate. See tests/test_stats.cpp for how a C++ test includes it.
 */
#ifndef STATS_H
#define STATS_H

#include <stddef.h>

/* Sum of values[0..n-1].
 * Stores the result in *out and returns 0.
 * Returns -1, leaving *out untouched, if the sum would overflow an int.
 * Preconditions (assert): values != NULL, out != NULL
 */
int stats_sum(const int *values, size_t n, int *out);

/* Arithmetic mean of values[0..n-1].
 * Stores the result in *out and returns 0.
 * Returns -1, leaving *out untouched, if n == 0.
 * Preconditions (assert): values != NULL, out != NULL
 */
int stats_mean(const int *values, size_t n, double *out);

/* Largest of values[0..n-1].
 * Stores the result in *out and returns 0.
 * Returns -1, leaving *out untouched, if n == 0.
 * Preconditions (assert): values != NULL, out != NULL
 */
int stats_max(const int *values, size_t n, int *out);

#endif /* STATS_H */
