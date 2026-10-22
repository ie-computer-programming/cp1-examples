/* 04 -- the point of threads: splitting work across cores, measured.
 *
 * Two jobs, each run with 1, 2, 4, 8 and 16 threads:
 *
 *   SUM    add up 33 million ints        -- limited by MEMORY bandwidth
 *   PRIMES count primes below 3 million  -- limited by ARITHMETIC
 *
 * Each thread works on its own slice and writes only its own slot, so no
 * thread ever touches another's data (contrast with 05).
 *
 * Timed with a WALL clock. clock() adds up CPU time over every thread, so
 * 4 threads busy for 1 second report 4 seconds.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_SUM     (1 << 25)        /* 33.5 million ints = 128 MB */
#define N_PRIMES  3000000
#define MAX_THREADS 16

typedef struct {
    const int *data;
    long       begin, end, stride;
    long long  result;
} Job;

static void *sum_slice(void *arg)
{
    Job *j = arg;
    long long acc = 0;
    for (long i = j->begin; i < j->end; i++) {
        acc += j->data[i];
    }
    j->result = acc;
    return NULL;
}

static int is_prime(long n)
{
    if (n < 2) return 0;
    for (long d = 2; d * d <= n; d++) {
        if (n % d == 0) return 0;
    }
    return 1;
}

/* Chunks dealt out round-robin: thread i takes chunks i, i+k, i+2k...
 * Interleaving single numbers would hand every even thread only even
 * numbers -- rejected instantly -- and leave the odd threads all the work. */
#define CHUNK 1000

static void *count_primes(void *arg)
{
    Job *j = arg;
    long long count = 0;
    for (long start = j->begin * CHUNK; start < N_PRIMES; start += j->stride * CHUNK) {
        for (long n = start; n < start + CHUNK && n < N_PRIMES; n++) {
            count += is_prime(n);
        }
    }
    j->result = count;
    return NULL;
}

static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static long long run(void *(*fn)(void *), const int *data, int k)
{
    pthread_t t[MAX_THREADS];
    Job       j[MAX_THREADS];
    for (int i = 0; i < k; i++) {
        j[i] = (Job){ data, (long)N_SUM * i / k, (long)N_SUM * (i + 1) / k, k, 0 };
        if (fn == count_primes) {
            j[i].begin = i;
        }
        pthread_create(&t[i], NULL, fn, &j[i]);
    }
    long long total = 0;
    for (int i = 0; i < k; i++) {
        pthread_join(t[i], NULL);
        total += j[i].result;       /* safe: read only after join */
    }
    return total;
}

static void bench(const char *label, void *(*fn)(void *), const int *data, int reps)
{
    double base = 0.0;
    printf("%s\nthreads   wall time   speed-up   CPU time (clock)\n", label);
    for (int k = 1; k <= MAX_THREADS; k *= 2) {
        clock_t c0 = clock();
        double  t0 = now();
        long long r = 0;
        for (int i = 0; i < reps; i++) {
            r = run(fn, data, k);
        }
        double wall = (now() - t0) / reps;
        double cpu  = (double)(clock() - c0) / CLOCKS_PER_SEC / reps;
        if (k == 1) {
            base = wall;
        }
        printf("%7d   %7.4f s   %6.2fx   %7.4f s   (%lld)\n", k, wall, base / wall, cpu, r);
    }
    printf("\n");
}

int main(void)
{
    int *data = malloc((size_t)N_SUM * sizeof *data);
    if (data == NULL) {
        perror("malloc");
        return 1;
    }
    for (long i = 0; i < N_SUM; i++) {
        data[i] = (int)(i % 7);
    }

    bench("SUM -- memory-bound", sum_slice, data, 20);
    bench("PRIMES -- compute-bound", count_primes, NULL, 2);

    free(data);
    return 0;
}
