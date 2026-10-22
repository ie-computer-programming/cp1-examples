/* 02-bad -- every line is locked, and the program is still wrong.
 *
 * A bank account must never go negative. withdraw() checks the balance and
 * then subtracts -- and each of those steps takes the lock. But the lock is
 * RELEASED between them. Two threads can both pass the check before either
 * subtracts: check-then-act.
 *
 * The critical section is not "each access to balance". It is everything
 * that must happen without interruption for the rule to hold.
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_THREADS 8

static long            balance = 100;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static long read_balance(void)
{
    pthread_mutex_lock(&lock);
    long b = balance;
    pthread_mutex_unlock(&lock);
    return b;
}

static void subtract(long amount)
{
    pthread_mutex_lock(&lock);
    balance -= amount;
    pthread_mutex_unlock(&lock);
}

static void *withdraw_100(void *arg)
{
    (void)arg;
    if (read_balance() >= 100) {      /* check ...                       */
        usleep(1000);                  /* ... (the real world takes time) */
        subtract(100);                 /* ... then act, on a stale check  */
    }
    return NULL;
}

int main(void)
{
    pthread_t t[N_THREADS];
    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&t[i], NULL, withdraw_100, NULL);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(t[i], NULL);
    }
    printf("8 withdrawals of 100 from 100 -> balance = %ld\n", balance);
    printf("(should be 0: exactly one withdrawal can succeed)\n");
    return 0;
}
