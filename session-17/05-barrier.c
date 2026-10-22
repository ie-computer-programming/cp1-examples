/* 05 -- a barrier: nobody starts phase 2 until everyone finishes phase 1.
 *
 * A simulation in steps. In each step every thread updates its own cells,
 * but reads its NEIGHBOURS' cells from the previous step. If a fast thread
 * runs ahead into step 2 while a slow one is still writing step 1, it reads
 * half-updated data.
 *
 * pthread_barrier_wait blocks until all N threads have arrived, then
 * releases them together. Exactly one of them gets
 * PTHREAD_BARRIER_SERIAL_THREAD back -- handy for "one thread prints".
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "barrier_compat.h"

#define N_THREADS 4
#define N_STEPS   3

static pthread_barrier_t barrier;

static void *simulate(void *arg)
{
    int id = *(const int *)arg;

    for (int step = 1; step <= N_STEPS; step++) {
        usleep((useconds_t)(1000 * (id + 1) * 5));     /* uneven work: thread 3 is slowest */
        printf("  step %d: thread %d done\n", step, id);

        if (pthread_barrier_wait(&barrier) == PTHREAD_BARRIER_SERIAL_THREAD) {
            printf("--- all %d threads finished step %d ---\n", N_THREADS, step);
        }
        pthread_barrier_wait(&barrier);   /* keep the banner ahead of the next step */
    }
    return NULL;
}

int main(void)
{
    pthread_barrier_init(&barrier, NULL, N_THREADS);

    pthread_t t[N_THREADS];
    int       ids[N_THREADS];
    for (int i = 0; i < N_THREADS; i++) {
        ids[i] = i;
        pthread_create(&t[i], NULL, simulate, &ids[i]);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(t[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}
