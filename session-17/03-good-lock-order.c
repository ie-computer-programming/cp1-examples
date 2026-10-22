/* 03-good -- every thread takes the locks in the SAME order.
 *
 * Pick one global order (here: accounts before audit_log) and never break
 * it. A deadlock needs a cycle: someone holding a LATER lock while waiting
 * for an EARLIER one. If everyone takes them in the same order, nobody ever
 * does that, so the cycle cannot form.
 */
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static pthread_mutex_t accounts  = PTHREAD_MUTEX_INITIALIZER;   /* lock #1 */
static pthread_mutex_t audit_log = PTHREAD_MUTEX_INITIALIZER;   /* lock #2 */

static void *transfer(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&accounts);
    usleep(1000);
    pthread_mutex_lock(&audit_log);
    puts("transfer: done");
    pthread_mutex_unlock(&audit_log);
    pthread_mutex_unlock(&accounts);
    return NULL;
}

static void *audit(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&accounts);      /* same order as transfer */
    usleep(1000);
    pthread_mutex_lock(&audit_log);
    puts("audit: done");
    pthread_mutex_unlock(&audit_log);
    pthread_mutex_unlock(&accounts);
    return NULL;
}

static void timeout(int sig)
{
    (void)sig;
    static const char msg[] = "DEADLOCK: no progress in 2 s -- killed by alarm\n";
    write(STDERR_FILENO, msg, sizeof msg - 1);
    _exit(1);
}

int main(void)
{
    signal(SIGALRM, timeout);
    alarm(2);

    pthread_t a, b;
    pthread_create(&a, NULL, transfer, NULL);
    pthread_create(&b, NULL, audit, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);

    puts("both finished");
    return 0;
}
