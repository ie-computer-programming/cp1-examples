/* pthread_barrier_t is an OPTIONAL part of POSIX and macOS does not ship it.
 * On Apple platforms (or when CP1_FORCE_BARRIER_SHIM is defined, for
 * testing) this provides a minimal stand-in built from a mutex and a
 * condition variable, under its own names, mapped onto the standard ones.
 * Everywhere else the real pthread barrier is used unchanged. */
#ifndef CP1_BARRIER_H
#define CP1_BARRIER_H

#include <pthread.h>

#if defined(__APPLE__) || defined(CP1_FORCE_BARRIER_SHIM)

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t  all_here;
    unsigned        needed, waiting, generation;
} cp1_barrier_t;

static inline int cp1_barrier_init(cp1_barrier_t *b, const void *attr, unsigned count)
{
    (void)attr;
    b->needed = count;
    b->waiting = 0;
    b->generation = 0;
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->all_here, NULL);
    return 0;
}

static inline int cp1_barrier_wait(cp1_barrier_t *b)
{
    pthread_mutex_lock(&b->lock);
    unsigned gen = b->generation;
    if (++b->waiting == b->needed) {
        b->generation++;                   /* last one in releases everyone */
        b->waiting = 0;
        pthread_cond_broadcast(&b->all_here);
        pthread_mutex_unlock(&b->lock);
        return -1;                         /* this thread plays SERIAL_THREAD */
    }
    while (gen == b->generation) {         /* loop: wake-ups can be spurious */
        pthread_cond_wait(&b->all_here, &b->lock);
    }
    pthread_mutex_unlock(&b->lock);
    return 0;
}

static inline int cp1_barrier_destroy(cp1_barrier_t *b)
{
    pthread_cond_destroy(&b->all_here);
    pthread_mutex_destroy(&b->lock);
    return 0;
}

#define pthread_barrier_t              cp1_barrier_t
#define pthread_barrier_init           cp1_barrier_init
#define pthread_barrier_wait           cp1_barrier_wait
#define pthread_barrier_destroy        cp1_barrier_destroy
#undef  PTHREAD_BARRIER_SERIAL_THREAD
#define PTHREAD_BARRIER_SERIAL_THREAD  (-1)

#endif
#endif /* CP1_BARRIER_H */
