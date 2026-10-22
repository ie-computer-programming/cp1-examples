/* 03-good -- one argument per thread, that outlives the thread.
 *
 * A struct per thread carries everything it needs in, and has room for the
 * result to come back out. The array lives in main, which joins every
 * thread before the array goes out of scope.
 */
#include <pthread.h>
#include <stdio.h>

#define N_THREADS 6

typedef struct {
    int id;
    int result;            /* written by the thread, read by main after join */
} WorkerArgs;

static void *worker(void *arg)
{
    WorkerArgs *a = arg;
    a->result = a->id * a->id;
    printf("I am thread %d\n", a->id);
    return NULL;
}

int main(void)
{
    pthread_t  threads[N_THREADS];
    WorkerArgs args[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        args[i] = (WorkerArgs){ .id = i };
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("  thread %d returned %d\n", args[i].id, args[i].result);
    }
    return 0;
}
