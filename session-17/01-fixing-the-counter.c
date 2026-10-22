/* 01 -- four ways to count to 4 000 000 with four threads, measured.
 *
 *   racy        counter++ on a shared global                (Session 16: wrong)
 *   mutex       lock, counter++, unlock -- every time      (right, slow)
 *   atomic      atomic_fetch_add                            (right, faster)
 *   local       count privately, lock ONCE at the end       (right, fastest)
 *
 * The lesson is not "use a mutex". It is: share as little as possible, and
 * when you must share, hold the lock for as short a time as possible.
 */
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <time.h>

#define N_THREADS  4
#define PER_THREAD 1000000

static long             counter;
static atomic_long      atomic_counter;
static pthread_mutex_t  lock = PTHREAD_MUTEX_INITIALIZER;

static void *racy(void *arg)
{
    (void)arg;
    for (int i = 0; i < PER_THREAD; i++) {
        counter++;
    }
    return NULL;
}

static void *with_mutex(void *arg)
{
    (void)arg;
    for (int i = 0; i < PER_THREAD; i++) {
        pthread_mutex_lock(&lock);       /* --- critical section --- */
        counter++;
        pthread_mutex_unlock(&lock);     /* ------------------------ */
    }
    return NULL;
}

static void *with_atomic(void *arg)
{
    (void)arg;
    for (int i = 0; i < PER_THREAD; i++) {
        atomic_fetch_add(&atomic_counter, 1);   /* load+add+store as ONE step */
    }
    return NULL;
}

static void *local_then_lock(void *arg)
{
    (void)arg;
    long mine = 0;                       /* on this thread's own stack */
    for (int i = 0; i < PER_THREAD; i++) {
        mine++;
    }
    pthread_mutex_lock(&lock);           /* one short critical section */
    counter += mine;
    pthread_mutex_unlock(&lock);
    return NULL;
}

static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void run(const char *label, void *(*fn)(void *), int use_atomic)
{
    pthread_t t[N_THREADS];
    counter = 0;
    atomic_store(&atomic_counter, 0);

    double t0 = now();
    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&t[i], NULL, fn, NULL);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(t[i], NULL);
    }
    double elapsed = now() - t0;

    long result = use_atomic ? atomic_load(&atomic_counter) : counter;
    printf("%-16s %9ld  %s   %7.4f s\n", label, result,
           result == (long)N_THREADS * PER_THREAD ? "correct" : "WRONG  ", elapsed);
}

int main(void)
{
    printf("method           result     verdict   time\n");
    run("racy",            racy,            0);
    run("mutex per ++",    with_mutex,      0);
    run("atomic",          with_atomic,     1);
    run("local, lock once", local_then_lock, 0);
    return 0;
}
