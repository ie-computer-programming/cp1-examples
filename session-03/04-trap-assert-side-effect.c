/* THE TRAP: a side effect hidden inside assert().
   Build twice and compare -- the program does different things.

     Debug   (asserts on) : next_value() is called once
     Release (NDEBUG)     : the whole line vanishes, it is called zero times */
#include <assert.h>
#include <stdio.h>

static int calls = 0;

static int next_value(void)
{
    calls++;
    return 42;
}

int main(void)
{
    assert(next_value() == 42);   /* WRONG: the call disappears under NDEBUG */

    printf("next_value() was called %d time(s)\n", calls);

#ifdef NDEBUG
    puts("built with NDEBUG -> assertions removed");
#else
    puts("built without NDEBUG -> assertions active");
#endif

    /* The fix: make the call, then assert the result.
           int rc = next_value();
           assert(rc == 42);                                          */

    return 0;
}
