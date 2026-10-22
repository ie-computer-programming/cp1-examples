/* 04 -- a semaphore: at most N threads inside at once.
 *
 * A mutex lets ONE thread in. A semaphore holds a count of permits:
 *   sem_wait  take a permit; if none are left, block until one is returned
 *   sem_post  give a permit back
 *
 * Here 8 workers share 3 "database connections". The semaphore guarantees
 * no more than 3 are ever in use, however the OS schedules the threads.
 *
 * Named semaphore (sem_open), not sem_init: macOS does not implement
 * unnamed semaphores -- sem_init compiles there and then fails at runtime.
 */
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N_WORKERS   8
#define CONNECTIONS 3

static sem_t           *pool;
static pthread_mutex_t  stats_lock = PTHREAD_MUTEX_INITIALIZER;
static int              in_use, peak;

static void *worker(void *arg)
{
    int id = *(const int *)arg;

    sem_wait(pool);                            /* take a connection */

    pthread_mutex_lock(&stats_lock);
    in_use++;
    if (in_use > peak) {
        peak = in_use;
    }
    printf("worker %d connected    (%d in use)\n", id, in_use);
    pthread_mutex_unlock(&stats_lock);

    usleep(20000);                             /* use it */

    pthread_mutex_lock(&stats_lock);
    in_use--;
    pthread_mutex_unlock(&stats_lock);

    sem_post(pool);                            /* give it back */
    return NULL;
}

int main(void)
{
    sem_unlink("/cp1-pool");                   /* discard any leftover from a crash */
    pool = sem_open("/cp1-pool", O_CREAT | O_EXCL, 0600, CONNECTIONS);
    if (pool == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    pthread_t t[N_WORKERS];
    int       ids[N_WORKERS];
    for (int i = 0; i < N_WORKERS; i++) {
        ids[i] = i;
        pthread_create(&t[i], NULL, worker, &ids[i]);
    }
    for (int i = 0; i < N_WORKERS; i++) {
        pthread_join(t[i], NULL);
    }

    printf("\n%d workers, %d connections -> peak in use: %d\n", N_WORKERS, CONNECTIONS, peak);

    sem_close(pool);
    sem_unlink("/cp1-pool");                   /* named: it outlives the process otherwise */
    return 0;
}
