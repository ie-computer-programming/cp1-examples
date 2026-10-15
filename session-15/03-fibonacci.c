/* 03 -- the same Fibonacci, three ways, measured.
 *
 * The textbook recursion is a perfect copy of the maths and a terrible
 * program: fib(n) calls fib(n-1) and fib(n-2), which call fib(n-2) and
 * fib(n-3) and fib(n-3) and fib(n-4)... recomputing the same values an
 * exponential number of times.
 *
 *   naive     elegant, exponential
 *   memo      same recursion + a table of answers already computed
 *   loop      no recursion at all
 */
#include <stdio.h>
#include <time.h>

#define N 40

static unsigned long long calls;

static unsigned long long fib_naive(int n)
{
    calls++;
    return (n < 2) ? (unsigned long long)n : fib_naive(n - 1) + fib_naive(n - 2);
}

static unsigned long long memo[N + 1];
static int known[N + 1];

static unsigned long long fib_memo(int n)
{
    calls++;
    if (n < 2) {
        return (unsigned long long)n;
    }
    if (!known[n]) {
        memo[n]  = fib_memo(n - 1) + fib_memo(n - 2);
        known[n] = 1;
    }
    return memo[n];
}

static unsigned long long fib_loop(int n)
{
    unsigned long long a = 0, b = 1;
    for (int i = 0; i < n; i++) {
        unsigned long long t = a + b;
        a = b;
        b = t;
    }
    return a;
}

static double secs(clock_t t0) { return (double)(clock() - t0) / CLOCKS_PER_SEC; }

int main(void)
{
    clock_t t;
    unsigned long long r;

    calls = 0; t = clock(); r = fib_naive(N);
    printf("naive  fib(%d) = %llu   %12llu calls   %.3f s\n", N, r, calls, secs(t));

    calls = 0; t = clock(); r = fib_memo(N);
    printf("memo   fib(%d) = %llu   %12llu calls   %.3f s\n", N, r, calls, secs(t));

    t = clock(); r = fib_loop(N);
    printf("loop   fib(%d) = %llu   %12s         %.3f s\n", N, r, "no", secs(t));
    return 0;
}
