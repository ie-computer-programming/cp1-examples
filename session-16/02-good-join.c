/* 02-good -- join every thread you create, before main returns. */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_THREADS 4

static void *slow_worker(void *arg)
{
    int id = *(const int *)arg;
    usleep(1000 * (useconds_t)(id + 1));
    printf("worker %d done\n", id);
    return NULL;
}

int main(void)
{
    pthread_t threads[N_THREADS];
    int       ids[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, slow_worker, &ids[i]) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            return 1;
        }
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);       /* blocks until thread i ends */
    }

    printf("main: returning after all workers\n");
    return 0;
}
