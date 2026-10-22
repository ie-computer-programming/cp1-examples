/* 03-bad -- deadlock: two locks, taken in opposite orders.
 *
 *   thread A: lock(accounts)  ...  lock(audit_log)
 *   thread B: lock(audit_log) ...  lock(accounts)
 *
 * A holds accounts and waits for audit_log. B holds audit_log and waits for
 * accounts. Neither will ever let go. The program does not crash -- it just
 * stops, forever, using no CPU.
 *
 * An alarm kills it after 2 seconds so this demo terminates.
 */
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static pthread_mutex_t accounts  = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t audit_log = PTHREAD_MUTEX_INITIALIZER;

static void *transfer(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&accounts);
    usleep(1000);                       /* widen the window */
    pthread_mutex_lock(&audit_log);
    puts("transfer: done");
    pthread_mutex_unlock(&audit_log);
    pthread_mutex_unlock(&accounts);
    return NULL;
}

static void *audit(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&audit_log);     /* OPPOSITE order */
    usleep(1000);
    pthread_mutex_lock(&accounts);
    puts("audit: done");
    pthread_mutex_unlock(&accounts);
    pthread_mutex_unlock(&audit_log);
    return NULL;
}

static void timeout(int sig)
{
    (void)sig;
    static const char msg[] = "DEADLOCK: no progress in 2 s -- killed by alarm\n";
    write(STDERR_FILENO, msg, sizeof msg - 1);    /* printf is unsafe in a signal handler */
    _exit(1);
}

int main(void)
{
    signal(SIGALRM, timeout);
    alarm(2);

    pthread_t a, b;
#ifdef SEQUENTIAL
    /* TSan build: one after the other, so it never hangs -- yet both lock
     * orders still get used, and TSan reports the potential deadlock. */
    pthread_create(&a, NULL, transfer, NULL);
    pthread_join(a, NULL);
    pthread_create(&b, NULL, audit, NULL);
    pthread_join(b, NULL);
#else
    pthread_create(&a, NULL, transfer, NULL);
    pthread_create(&b, NULL, audit, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
#endif

    puts("both finished");
    return 0;
}
