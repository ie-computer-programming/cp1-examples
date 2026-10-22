/* 02-bad -- main returns without joining.
 *
 * Returning from main ends the PROCESS, and every thread in it with it --
 * mid-sentence. How much of the workers' output you see depends on timing:
 * sometimes some, often none. Run it a few times.
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_THREADS 4

static void *slow_worker(void *arg)
{
    int id = *(const int *)arg;
    usleep(1000 * (useconds_t)(id + 1));       /* a little real work */
    printf("worker %d done\n", id);
    return NULL;
}

int main(void)
{
    pthread_t threads[N_THREADS];
    int       ids[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, slow_worker, &ids[i]);
    }

    printf("main: returning   <-- no pthread_join\n");
    return 0;
}
