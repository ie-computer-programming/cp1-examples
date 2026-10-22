/* 02-good -- one lock around the whole check-and-act. */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_THREADS 8

static long            balance = 100;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Returns 1 if the money was withdrawn, 0 if refused. */
static int withdraw(long amount)
{
    int ok = 0;
    pthread_mutex_lock(&lock);
    if (balance >= amount) {           /* check ...                  */
        usleep(1000);
        balance -= amount;             /* ... and act, as ONE unit   */
        ok = 1;
    }
    pthread_mutex_unlock(&lock);       /* on every path, including refusal */
    return ok;
}

static void *withdraw_100(void *arg)
{
    int *succeeded = arg;
    *succeeded = withdraw(100);
    return NULL;
}

int main(void)
{
    pthread_t t[N_THREADS];
    int       succeeded[N_THREADS];
    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&t[i], NULL, withdraw_100, &succeeded[i]);
    }
    int total = 0;
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(t[i], NULL);
        total += succeeded[i];
    }
    printf("8 withdrawals of 100 from 100 -> balance = %ld, %d succeeded\n", balance, total);
    return 0;
}
