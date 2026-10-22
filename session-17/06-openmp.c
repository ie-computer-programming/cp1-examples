/* 06 -- OpenMP: the same parallel sum as Session 16, in one line.
 *
 * Session 16's 04-parallel-sum needed a struct per thread, pthread_create,
 * pthread_join and a final combine. OpenMP is a set of compiler directives
 * that generates all of that for you:
 *
 *   #pragma omp parallel for reduction(+:sum)
 *
 * "parallel for" splits the loop across threads; "reduction(+:sum)" gives
 * each thread a private sum and adds them up at the end -- exactly the
 * slot-per-thread pattern, written by the compiler.
 *
 * Build with -fopenmp. Without it, the pragma is ignored and the loop runs
 * on one thread -- same answer, no speed-up.
 */
#include <omp.h>
#include <stdio.h>

#define N_PRIMES 3000000

static int is_prime(long n)
{
    if (n < 2) return 0;
    for (long d = 2; d * d <= n; d++) {
        if (n % d == 0) return 0;
    }
    return 1;
}

int main(void)
{
    for (int threads = 1; threads <= 8; threads *= 2) {
        omp_set_num_threads(threads);
        double t0 = omp_get_wtime();               /* a wall clock, as in Session 16 */

        long count = 0;
        #pragma omp parallel for reduction(+:count) schedule(dynamic, 1000)
        for (long n = 0; n < N_PRIMES; n++) {
            count += is_prime(n);
        }

        printf("%d thread(s): %ld primes in %.4f s\n", threads, count, omp_get_wtime() - t0);
    }
    return 0;
}
