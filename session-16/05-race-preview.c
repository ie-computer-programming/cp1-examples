/* 05 -- a preview of Session 17: four threads, one shared counter.
 *
 * Each thread adds 1 to `counter` a million times. The answer should be
 * 4 000 000. Run it a few times.
 *
 * counter++ is not one step. It is three: load counter into a register,
 * add 1, store it back. Two threads can both load 41, both add 1, and both
 * store 42 -- one increment lost. Nothing crashes, nothing warns, and the
 * answer changes every run.
 *
 * This file has no "good" twin on purpose. Fixing it is Session 17.
 */
#include <pthread.h>
#include <stdio.h>

#define N_THREADS 4
#define PER_THREAD 1000000

static long counter = 0;           /* shared by every thread */

static void *increment(void *arg)
{
    (void)arg;
    for (int i = 0; i < PER_THREAD; i++) {
        counter++;                 /* load, add, store -- interruptible */
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    long expected = (long)N_THREADS * PER_THREAD;
    printf("counter = %ld   expected %ld   lost %ld increments\n",
           counter, expected, expected - counter);
    return 0;
}
