/* 07 -- atomics: one indivisible operation, no lock.
 *
 * * Part 1: a ticket dispenser. Each thread takes 250 000 tickets; every ticket
 * number must be handed out exactly once.
 *
 *   plain    t = next++                    load, add, store -- interruptible
 *   atomic   t = atomic_fetch_add(&next,1) one step, and it RETURNS the old value
 *
 * That return value is what a bare counter++ cannot give you safely: "the
 * number I just took, that no other thread can also have taken".
 *
 * Part 2: why an atomic is still slower than a private counter. An atomic
 * add is cheap while one thread does it. With four threads hammering the same
 * variable, the cores keep taking the memory away from each other.
 *
 * Built at -O0 on purpose (see 01): the optimiser can hide a data race.
 */
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define N_THREADS 4
#define PER_THREAD 250000                /* enough that the threads really overlap */
#define N_TICKETS (N_THREADS * PER_THREAD)

static int        plain_next;
static atomic_int atomic_next;
static int        got[N_THREADS][PER_THREAD];

static void *take_plain(void *arg)
{
    int id = *(const int *)arg;
    for (int k = 0; k < PER_THREAD; k++) {
        got[id][k] = plain_next++;                       /* load, add, store */
    }
    return NULL;
}

static void *take_atomic(void *arg)
{
    int id = *(const int *)arg;
    for (int k = 0; k < PER_THREAD; k++) {
        got[id][k] = atomic_fetch_add(&atomic_next, 1);  /* returns the OLD value */
    }
    return NULL;
}

static void run_tickets(const char *label, void *(*fn)(void *))
{
    pthread_t t[N_THREADS];
    int       ids[N_THREADS];
    for (int i = 0; i < N_THREADS; i++) {
        ids[i] = i;
        pthread_create(&t[i], NULL, fn, &ids[i]);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(t[i], NULL);
    }

    static int seen[N_TICKETS];
    memset(seen, 0, sizeof seen);
    int duplicated = 0;
    for (int i = 0; i < N_THREADS; i++) {
        for (int k = 0; k < PER_THREAD; k++) {
            if (seen[got[i][k]]++ >= 1) {
                duplicated++;               /* someone already holds this ticket */
            }
        }
    }
    printf("%-7s %7d tickets taken, %7d duplicates   %s\n",
           label, N_TICKETS, duplicated, duplicated ? "WRONG" : "correct");
}

static atomic_long shared;
#define ADDS 4000000

static void *add_many(void *arg)
{
    long n = *(const long *)arg;
    for (long i = 0; i < n; i++) {
        atomic_fetch_add(&shared, 1);
    }
    return NULL;
}

static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static double time_adds(int threads)
{
    pthread_t t[N_THREADS];
    long      each = ADDS / threads;
    atomic_store(&shared, 0);

    double t0 = now();
    for (int i = 0; i < threads; i++) {
        pthread_create(&t[i], NULL, add_many, &each);
    }
    for (int i = 0; i < threads; i++) {
        pthread_join(t[i], NULL);
    }
    return now() - t0;
}

int main(void)
{
    printf("PART 1: ticket dispenser\n");
    run_tickets("plain", take_plain);
    run_tickets("atomic", take_atomic);

    printf("\nPART 2: %d atomic adds on ONE variable\n", ADDS);
    double one  = time_adds(1);
    double four = time_adds(4);
    printf("1 thread   %.4f s   (nobody to fight with)\n", one);
    printf("4 threads  %.4f s   (same total work, %.1fx slower)\n", four, four / one);
    return 0;
}
