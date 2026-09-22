/* 03-good -- pass a POINTER when the function must modify the struct,
 * or when the struct is big enough that copying it is waste.
 *
 * Rule of thumb:
 *   Account *a        the function may modify it
 *   const Account *a  the function only reads it -- and the compiler enforces that
 *
 * The `const` version is not decoration. It is the only thing in the
 * signature that tells a caller whether their data is safe.
 */
#include <stdio.h>

typedef struct {
    double balance;
    char   owner[32];
} Account;

static void deposit(Account *account, double amount)
{
    account->balance += amount;
}

static void print_account(const Account *account)
{
    printf("  %s: %.2f\n", account->owner, account->balance);
    /* account->balance = 0.0;  <-- error: assignment of member in read-only object */
}

int main(void)
{
    Account a = {.balance = 100.0, .owner = "Ada"};

    printf("before :\n");
    print_account(&a);

    deposit(&a, 50.0);

    printf("after  :\n");
    print_account(&a);

    return 0;
}
