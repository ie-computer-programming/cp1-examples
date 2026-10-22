/* 01 -- create four threads, wait for all of them.
 *
 *   pthread_create(&id, attributes, function, argument)   start one
 *   pthread_join(id, &result)                              wait for it
 *
 * Run it several times. The ORDER of the "hello" lines changes from run to
 * run: once started, the threads are scheduled by the operating system,
 * not by your source code. Python's threading.Thread(target=f).start() and
 * .join() are a thin wrapper around exactly these two calls.
 */
#include <pthread.h>
#include <stdio.h>

#define N_THREADS 4

static void *say_hello(void *arg)
{
    int id = *(const int *)arg;
    printf("hello from thread %d\n", id);
    return NULL;
}

int main(void)
{
    pthread_t threads[N_THREADS];
    int       ids[N_THREADS];          /* one argument per thread -- see 03 */

    for (int i = 0; i < N_THREADS; i++) {
        ids[i] = i;
        int rc = pthread_create(&threads[i], NULL, say_hello, &ids[i]);
        if (rc != 0) {                 /* returns an error number, not -1 + errno */
            fprintf(stderr, "pthread_create: error %d\n", rc);
            return 1;
        }
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("main: all %d threads finished\n", N_THREADS);
    return 0;
}
