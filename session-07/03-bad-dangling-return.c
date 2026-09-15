/* BAD: returning the address of a local variable.
 *
 * `answer` lives in make_answer's stack frame. The moment the function
 * returns, that frame is gone and the address is meaningless. The pointer
 * still LOOKS fine -- it is a plausible-looking address -- which is exactly
 * what makes this bug so hard to see.
 *
 * gcc and clang both warn here. Read your warnings.
 * Build the -asan variant to watch it caught at runtime:
 *     ./build/03-bad-dangling-return-asan
 */
#include <stdio.h>

static int *make_answer(void)
{
    int answer = 42;
    return &answer;      /* <-- the bug */
}

int main(void)
{
    int *p = make_answer();

    /* Might print 42. Might print garbage. Might crash. All three are
     * "correct" behaviour for undefined behaviour. */
    printf("the answer is %d\n", *p);
    return 0;
}
