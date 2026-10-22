/* 03-bad -- passing the address of the loop variable.
 *
 * Every thread receives &i: the SAME address. By the time a thread gets
 * round to reading it, the loop has usually moved on -- so several threads
 * see the same id, and some ids are never seen at all. When the loop ends,
 * `i` is out of scope and the pointer dangles (Session 7 again).
 */
#include <pthread.h>
#include <stdio.h>

#define N_THREADS 6

static void *worker(void *arg)
{
    int id = *(const int *)arg;      /* reads whatever i is NOW */
    printf("I am thread %d\n", id);
    return NULL;
}

int main(void)
{
    pthread_t threads[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, &i);    /* one shared address */
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
