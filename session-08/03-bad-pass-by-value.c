/* 03-bad -- passing a struct to a function copies it.
 *
 * This is the Python surprise in reverse. In Python, handing an object to a
 * function hands over a reference and the function can mutate it. In C, a
 * struct argument is COPIED, member by member, and the function mutates the
 * copy. The original is untouched, silently.
 *
 * Nothing crashes. Nothing warns. The program just does not work.
 */
#include <stdio.h>

typedef struct {
    double balance;
    char   owner[32];
} Account;

/* Takes a COPY of the whole Account -- 40 bytes memcpy'd onto the stack. */
static void deposit(Account account, double amount)
{
    account.balance += amount;
    printf("  inside deposit(): balance = %.2f\n", account.balance);
}

int main(void)
{
    Account a = {.balance = 100.0, .owner = "Ada"};

    printf("before : %.2f\n", a.balance);
    deposit(a, 50.0);
    printf("after  : %.2f   <-- unchanged\n", a.balance);

    printf("exit code will be 0 -- the bug is silent\n");
    return 0;
}
