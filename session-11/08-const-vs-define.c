/* 08 -- for a constant, a macro is usually the worst of the three options.
 *
 *   #define MAX_USERS 100   text. No type, no scope, invisible to the debugger
 *   const int max_users     typed and scoped, but NOT a compile-time constant in C
 *   enum { MAX_USERS = 100 } typed, scoped, compile-time, debugger-visible
 *
 * For integers, the enum is usually the right answer and almost nobody
 * reaches for it. For anything non-integer (a double, a string) you still
 * need const or a macro.
 */
#include <stdio.h>

#define  MACRO_MAX   100              /* no type at all */
const int CONST_MAX = 100;            /* int, with linkage and an address */
enum    { ENUM_MAX  = 100 };          /* int, compile-time constant */

int main(void)
{
    printf("MACRO_MAX = %d\n", MACRO_MAX);
    printf("CONST_MAX = %d  (has an address: %p)\n",
           CONST_MAX, (const void *)&CONST_MAX);
    printf("ENUM_MAX  = %d\n", ENUM_MAX);

    /* Both of these are legal array sizes. `const int` is NOT a constant
     * expression in C, so `int c[CONST_MAX];` is a variable-length array --
     * a different feature with different rules. */
    int a[MACRO_MAX];
    int b[ENUM_MAX];
    printf("sizeof a = %zu, sizeof b = %zu\n", sizeof a, sizeof b);

    /* The hazard a macro has and the other two do not: it is not scoped, so
     * it replaces that name EVERYWHERE after this line -- including inside
     * other people's headers.
     *
     *     #define count 10
     *     struct bag { int count; };   <-- error: expected identifier
     *
     * This is why macro names are SHOUTED. The capitals are a warning that
     * ordinary language rules do not apply. */
    return 0;
}
